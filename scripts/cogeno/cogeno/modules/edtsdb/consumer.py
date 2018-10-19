#
# Copyright (c) 2018 Bobby Noelte
# Copyright (c) 2018 Linaro Limited
#
# SPDX-License-Identifier: Apache-2.0
#

import json
from pathlib import Path

from .devices.device import EDTSDevice
from .devices.pincontroller import EDTSPinController

##
# @brief ETDS Database consumer
#
# Methods for ETDS database usage.
#
class EDTSConsumerMixin(object):
    __slots__ = []

    ##
    # @brief Get compatibles
    #
    # @param None
    # @return edts 'compatibles' dict
    def get_compatibles(self):
        return self._edts['compatibles']

    ##
    # @brief Get aliases
    #
    # @param None
    # @return edts 'aliases' dict
    def get_aliases(self):
        return self._edts['aliases']

    ##
    # @brief Get chosen
    #
    # @param None
    # @return edts 'chosen' dict
    def get_chosen(self):
        return self._edts['chosen']

    ##
    # @brief Get device types
    #
    # @param None
    # @return edts device types dict
    def get_device_types(self):
        return self._edts['device-types']

    ##
    # @brief Get controllers
    #
    # @param  None
    # @return compatible generic device type
    def get_controllers(self):
        return self._edts['controllers']

    ##
    # @brief Get device ids of all activated compatible devices.
    #
    # @param compatibles compatible(s)
    # @return list of device ids of activated devices that are compatible
    def get_device_ids_by_compatible(self, compatibles):
        device_ids = dict()
        if not isinstance(compatibles, list):
            compatibles = [compatibles, ]
        for compatible in compatibles:
            for device_id in self._edts['compatibles'].get(compatible, []):
                device_ids[device_id] = 1
        return list(device_ids.keys())

    def get_devices_by_compatible(self, compatibles):
        devices = []
        device_ids = self.get_device_ids_by_compatible(compatibles)
        for device_id in device_ids:
            devices.append(self.get_device_by_device_id(device_id))
        return devices

    ##
    # @brief Get device id of activated device with given label.
    #
    # @param label
    # @return device id
    def get_device_id_by_label(self, label):
        for device_id, device in self._edts['devices'].items():
            if label == device.get('label', None):
                return device_id
        print("consumer.py: Device with label",
               "'{}' not available in EDTS".format(label))
        return None

    ##
    # @brief Get the device object matching a device_id.
    #
    # @param device_id
    # @return (dict)device
    def get_device_by_device_id(self, device_id):
        if not device_id in self._edts['devices'].keys():
            print("consumer.py: Device with device id",
                  "'{}' not available in EDTS".format(device_id))
            return None
        if 'pin-controller' in self._edts['devices'][device_id].keys():
            return EDTSPinController(self, device_id)
        return EDTSDevice(self, device_id)

    ##
    # @brief Get all activated compatible devices.
    #
    # @param compatibles compatible(s)
    # @return list of activated devices that are compatible
    def get_devices_by_compatible(self, compatibles):
        devices = []
        device_ids = self.get_device_ids_by_compatible(compatibles)
        for device_id in device_ids:
            devices.append(self.get_device_by_device_id(device_id))
        return devices

    def load(self, file_path):
        with Path(file_path).open(mode = "r", encoding="utf-8") as load_file:
            self._edts = json.load(load_file)
