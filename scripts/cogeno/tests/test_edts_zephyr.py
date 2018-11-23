#!/usr/bin/env python3
#
# Copyright (c) 2018 Linaro Limited
#
# SPDX-License-Identifier: Apache-2.0
#

import os
import argparse
import json
import pprint
import subprocess
import logging
from pathlib import Path
from copy import deepcopy
from os import path
import sys

if "ZEPHYR_BASE" not in os.environ:
    logging.error("$ZEPHYR_BASE environment variable undefined.\n")
    exit(1)

zephyr_path = Path(os.environ.get('ZEPHYR_BASE')).resolve()

tests_path = Path(__file__).resolve().parent
cogeno_path = tests_path.parent.joinpath('cogeno')
cogeno_modules_path = cogeno_path.joinpath('modules')

sys.path.append(str(cogeno_modules_path))

from edtsdatabase import EDTSDatabase

logger = None
test_data_path = tests_path.joinpath('test_edts_zephyr_data')
app_path = test_data_path.joinpath('dummy_app')
app_build_path = app_path.joinpath('_build')
board = 'refboard'


def init_logs():
    global logger
    log_lev = os.environ.get('LOG_LEVEL', None)
    level = logging.INFO
    if log_lev == "DEBUG":
        level = logging.DEBUG
    elif log_lev == "ERROR":
        level = logging.ERROR

    console = logging.StreamHandler()
    format = logging.Formatter('%(levelname)-8s: %(message)s')
    console.setFormatter(format)
    logger = logging.getLogger('')
    logger.addHandler(console)
    logger.setLevel(level)

    logging.debug("Log init completed")

def run_edts_build():
    if app_build_path.is_dir():
        cmd ="rm -rf {}".format(app_build_path)
        proc = subprocess.Popen(cmd, shell=True)
        proc.communicate()
    app_build_path.mkdir()

    cmd = 'PYTHONPATH= && source {}/zephyr-env.sh'.format(zephyr_path)
    cmd += ' && cmake -GNinja -DBOARD={} ..'.format(board)
    logger.debug('Sanity(%s)   %s' %(None, cmd))

    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE,
                            cwd=str(app_build_path), shell=True,
                            executable='/bin/bash')
    output,error=proc.communicate()
    if proc.wait() == 0:
        logger.debug(output)
        return True
    assert 0, output.decode('utf8') # to show value
    # logger.error(output)
    # raise Exception("Couldn't build dummy app in edtscheck")


class EDTScheck(object):

    def __init__(self, ref_path, gen_path, compiled_dts_path, bindings):
        self._edtstocheck = EDTSDatabase()
        self._edtstocheck.extract(compiled_dts_path, bindings)
        self._edtstocheck.save(gen_path)

        self._edtsref = EDTSDatabase()
        self._edtsref.load(ref_path)


    def compare_lists(self, edts_list, ref_list, list_name):
        message = ''
        local_edts_list = deepcopy(list(edts_list))
        for key in ref_list:
            if key not in local_edts_list:
                message += "\n'%s' missing in %s" % (key,list_name)
            else:
                local_edts_list.remove(key)
        for key in local_edts_list:
            message += "\n'%s' does not expect %s" % (list_name,key)
        return message


    def compare_dicts(self, edts_dict, ref_dict, dict_name):
        message = ''
        for key in ref_dict:
            if ref_dict[key] != edts_dict[key]:
                message += "\n %s[%s] = %s, expects %s" % \
                            (dict_name,key,edts_dict[key],ref_dict[key])
        return message


    def main(self):
        status_message = ''

        # Check database structure
        edts_struct = dict(self._edtstocheck)
        ref_struct = dict(self._edtsref)
        status_message += self.compare_lists(edts_struct, ref_struct,
                                             "Structure")
        if status_message is not '':
            logger.error(status_message)
            return

        # Check supported device list
        edts_device_types = self._edtstocheck.get_device_types()
        ref_device_types = self._edtsref.get_device_types()
        status_message += self.compare_lists(edts_device_types,
                                             ref_device_types,
                                             "Device types")

        # Check supported compatibles
        edts_compatibles = self._edtstocheck.get_compatibles()
        ref_compatibles = self._edtsref.get_compatibles()
        status_message += self.compare_lists(edts_compatibles,
                                             ref_compatibles,
                                             "Compatibles")

        # Check aliases
        aliases_message = ''
        edts_aliases = self._edtstocheck.get_aliases()
        ref_aliases = self._edtsref.get_aliases()
        aliases_message += self.compare_lists(edts_aliases,
                                             ref_aliases,
                                             "Aliases")
        if aliases_message is '':
            for alias in list(ref_aliases):
                aliases_message += self.compare_lists(edts_aliases[alias],
                                                      ref_aliases[alias],
                                                      "%s" % alias)
        status_message += aliases_message

        # Check chosen
        chosen_message = ''
        edts_chosen = self._edtstocheck.get_chosen()
        ref_chosen = self._edtsref.get_chosen()
        chosen_message += self.compare_lists(edts_chosen,
                                             ref_chosen,
                                             "chosen")
        if chosen_message is '':
            chosen_message += self.compare_dicts(edts_chosen, ref_chosen, \
                                                 'chosen')
        status_message += chosen_message

        # Check controllers
        controller_message = ''
        edts_controllers = self._edtstocheck.get_controllers()
        ref_controllers = self._edtsref.get_controllers()
        controller_message += self.compare_lists(edts_controllers,
                                                 ref_controllers,
                                                 "Controllers")
        if controller_message is '':
            for controller_type in list(ref_controllers):
                controller_message += \
                        self.compare_lists(edts_controllers[controller_type],
                                           ref_controllers[controller_type],
                                           "%s" % controller_type)
        status_message += controller_message

        # Stop here if an error was detected
        if status_message is not '':
            logger.error(status_message)
            return

        # Continue with devices
        ref_devices = []
        edts_devices = []
        for compat in ref_compatibles:
            ref_devices.extend(self._edtsref.get_device_ids_by_compatible(compat))
            edts_devices.extend(self._edtstocheck.get_device_ids_by_compatible(compat))
            status_message += self.compare_lists(edts_devices,
                                                 ref_devices,
                                                 "Devices")
        if status_message is not '':
            logger.error(status_message)
            return

        # Devices list is ok, check devices parameters
        device_list_message = ''
        for dev_id in ref_devices:
            ref_device = self._edtsref.get_device_by_device_id(dev_id)
            device_to_check = self._edtstocheck.get_device_by_device_id(dev_id)
            flatten_ref_device = ref_device.get_properties_flattened()
            flattend_device_to_check = device_to_check.get_properties_flattened()
            # First check lists of available properties for this device
            device_list_message += self.compare_lists(flattend_device_to_check,
                                                 flatten_ref_device,
                                                 "%s parameters" % dev_id)
            if device_list_message is not '':
                status_message += device_list_message
                device_list_message = ''
                continue
            # Now check properties values
            for prop in flatten_ref_device:
                value = device_to_check.get_property(prop, '')
                if value != flatten_ref_device[prop]:
                    status_message += '\n%s[%s] = %s, expected %s' % \
                                    (dev_id,prop,value,flatten_ref_device[prop])
            # Now check device parent<->child relations
            self.compare_lists(device_to_check.get_children(),
                               ref_device.get_children(),
                               "%s children" % dev_id)

        return status_message


def test_edts_zephyr():
    ref_path = test_data_path.joinpath('edts_ref.json')
    gen_path = app_build_path.joinpath('edts_gen.json')
    compiled_dts_path = app_build_path.joinpath('zephyr', '{}.dts_compiled'.format(board))
    bindings = [app_path.joinpath('dts', 'bindings'), zephyr_path.joinpath('dts', 'bindings'), ]
    init_logs()
    run_edts_build()
    status = EDTScheck(ref_path, gen_path, compiled_dts_path, bindings).main()
    assert status is ''
