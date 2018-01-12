#
# Copyright (c) 2018 Bobby Noelte
#
# SPDX-License-Identifier: Apache-2.0
#

from extract.globals import *
from extract.directive import DTDirective

##
# @brief Manage compatible directives.
#
class DTCompatible(DTDirective):

    def __init__(self):
        self._data = {}
        ##
        # Dictionary of all compatible strings
        # key is the compatible label, value is the compatible id
        self._compatible = { "UNKNOWN" : 0, }
        ##
        # Number of compatible strings
        self._compatible_count = 1

    ##
    # @brief Extract compatible
    #
    # compatible information is added to the soc node
    # - SOC_COMPATIBLE_ID_<compatible_label>: <index>
    # - SOC_COMPATIBLE_COUNT: <index> + 1
    #
    # and compatible information is added to the device node
    # - [device_label]_COMPATIBLE_<index>_ID: <soc_compatible_index>
    # - [device_label]_COMPATIBLE_COUNT: <index> + 1
    #
    # @param node_address Address of node owning the
    #                     compatible definition.
    # @param yaml YAML definition for the owning node.
    # @param y_key compatible key
    # @param names (unused)
    # @param[out] defs Property definitions for each node address
    # @param def_label Define label string of node owning the
    #                  compatible definition.
    #
    def extract(self, node_address, yaml, y_key, names, defs, def_label):

        # compatible definition
        compatible = reduced[node_address]['props'][y_key]
        if not isinstance(compatible, list):
            compatible = [compatible]
        device_label = get_node_label_string(node_address)

        # pseudo address for SoC node
        soc_node_address = "soc"
        # pseudo label for SoC node
        soc_label = "SOC"

        # Get the index for the compatible info for this node
        # (precaution if there are several compatible directives in a node)
        index_label = get_label_string( \
                    [device_label, "COMPATIBLE", "COUNT"])
        index = self._data.get(index_label, 0)
        self._data[index_label] = index + 1

        prop_def = {}
        for comp in compatible:
            # sanitize string
            comp = get_label_string([comp])
            if comp in self._compatible:
                soc_compatible_index = self._compatible[comp]
            else:
                # add new compatible string
                soc_compatible_index = len(self._compatible)
                self._compatible[comp] = soc_compatible_index

            compatible_id_label = get_label_string([device_label, "COMPATIBLE",
                                                    str(index), "ID"])
            prop_def[compatible_id_label] = soc_compatible_index
            index = index + 1

        self._data[index_label] = index
        prop_def[index_label] = self._data[index_label]

        # update node of compatible directive with compatible info
        insert_defs(node_address, defs, prop_def, {})

        # update soc node with compatible info
        prop_def = {}
        for comp in self._compatible:
            compatible_label = get_label_string( \
                                [soc_label, "COMPATIBLE", "ID", comp])
            prop_def[compatible_label] = str(self._compatible[comp])
        index_label = get_label_string( \
                                [soc_label, "COMPATIBLE", "COUNT"])
        prop_def[index_label] = len(self._compatible)
        insert_defs(soc_node_address, defs, prop_def, {})

        return

