#
# Copyright (c) 2018 Linaro Limited
#
# SPDX-License-Identifier: Apache-2.0
#

import pprint

from .directive import DTDirective

##
# @brief Manage gpio-x directive.
# insert <gpio-prop-name>-_controller
# insert gpio cells based on gpio-controller definitions
#
class DTGpios(DTDirective):

    def __init__(self, extractors):
        super().__init__(extractors)

    ##
    # @brief Extract gpio information.
    #
    # @param node_address Address of node owning the pinctrl definition.
    # @param prop gpio key
    #
    def _extract_controller(self, node_address, prop, prop_values, index):
        # get controller node (referenced via phandle)
        cell_parent = self.dts().phandles[prop_values[0]]

        for k in self.dts().reduced[cell_parent]['props']:
            if k[0] == '#' and '-cells' in k:
                num_cells = self.dts().reduced[cell_parent]['props'].get(k)

        try:
           l_cell = self.dts().reduced[cell_parent]['props'].get('label')
        except KeyError:
            l_cell = None

        if l_cell is not None:
            self.edts().insert_device_property(edts_device_id(node_address),
                    '{}/{}/controller'.format(prop, index),
                    cell_parent)

            self.edts_insert_device_parent_device_property(node_address)

        # prop off phandle + num_cells to get to next list item
        prop_values = prop_values[num_cells+1:]

        return cell_parent


    def _extract_cells(self, node_address, prop, prop_values, index,
		       controller):

        cell_parent = self.dts().phandles[prop_values.pop(0)]

        try:
            parent_binding = self.bindings()[get_compat(cell_parent)]
        except:
            raise Exception(
                "Could not find binding for " +
                    self.dts().reduced[cell_parent]['name'])

        # Get number of cells per element of current property
        for k in self.dts().reduced[cell_parent]['props'].keys():
            if k[0] == '#' and '-cells' in k:
                num_cells = self.dts().reduced[cell_parent]['props'].get(k)

        for i in range(num_cells):
            cell_name = parent_binding['#cells'][i]
            cell_value = prop_values.pop(0)

            self.edts().insert_device_property(edts_device_id(node_address),
                    '{}/{}/{}'.format(prop, index, cell_name), cell_value)

            if 'pin' in cell_name:
                self.edts_insert_device_controller(controller, node_address,
                                              cell_value)

        return prop_values


    ##
    # @brief Extract gpio related directives
    #
    # @param node_address Address of node issuing the directive.
    # @param prop Directive property name
    #
    def extract(self, node_address, prop):

        try:
            prop_values = list(self.dts().reduced[node_address]['props'].get(prop))
        except:
            prop_values = self.dts().reduced[node_address]['props'].get(prop)

        if 'gpio' in prop:
            # indicator for clock consumers
            index = 0
            while len(prop_values) > 0:
                controller = self._extract_controller(node_address, prop,
                                                      prop_values, index)
                prop_values = self._extract_cells(node_address, prop,
                                                  prop_values, index,
                                                  controller)
                index += 1
        else:
            raise Exception(
                "DTGpios.extract called with unexpected directive ({})."
                    .format(prop))
