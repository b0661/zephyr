#
# Copyright (c) 2018 Bobby Noelte
#
# SPDX-License-Identifier: Apache-2.0
#

from extract.globals import *
from extract.directive import DTDirective

##
# @brief Manage <device>-controller directives.
#
class DTController(DTDirective):

    def __init__(self):
        self._data = {}

    ##
    # @brief Extract <device>-controller
    #
    # <device>-controller information is added to the SoC node
    # - SOC_<device>_CONTROLLER_<index>: <device_label>
    # - SOC_<device>_CONTROLLER_COUNT: <index> + 1
    #
    # @param node_address Address of node owning the
    #                     <device>-controller definition.
    # @param yaml YAML definition for the owning node.
    # @param y_key <device>-controller key
    # @param names (unused)
    # @param[out] defs Property definitions for each node address
    # @param def_label Define label string of node owning the
    #                  <device>-controller definition.
    #
    def extract(self, node_address, yaml, y_key, names, defs, def_label):

        # <device>-controller definition
        device = y_key.replace('-controller', '')
        device_label = get_node_label_string(node_address)

        # pseudo address for SoC node
        soc_node_address = "soc"
        # pseudo label for SoC node
        soc_label = "SOC"

        # Get the index for the <device>-controller info
        index_label = get_label_string( \
                    [soc_label, device, "CONTROLLER", "COUNT"])
        index = self._data.get(index_label, 0)
        self._data[index_label] = index + 1

        prop_def = {}
        prop_def[index_label] = self._data[index_label]

        controller_label = get_label_string([soc_label, device, "CONTROLLER", str(index)])
        prop_def[controller_label] = device_label

        insert_defs(soc_node_address, defs, prop_def, {})

        return

