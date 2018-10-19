#
# Copyright (c) 2018 Bobby Noelte
#
# SPDX-License-Identifier: Apache-2.0
#

from ..dts import *
from ..bindings import *

##
# @brief Base class for pool of extractors for device tree data extraction.
#
class ExtractorsBase(object):

    def __init__(self, edts, dts, bindings):
        self._edts = edts
        self._dts = dts
        self._bindings = bindings
        # Extractors
        self.clocks = None
        self.compatible = None
        self.controller = None
        self.default = None
        self.dma = None
        self.flash = None
        self.gpioranges = None
        self.gpios = None
        self.heuristics = None
        self.interrupts = None
        self.pinctrl = None
        self.pwm = None
        self.reg = None
        ##
        # Properties that the extractors want to be ignored
        # because they are unnecessary or are covered by the
        # extraction of other properties
        self.ignore_properties = []

    # convenience functions

    def dts(self):
        return self._dts

    def edts(self):
        return self._edts

    def bindings(self):
        return self._bindings

##
# @brief Base class for device tree directives extraction to edts.
#
class DTDirective(object):

    def __init__(self, extractors):
        ## The extractor pool this extractor belongs to
	    self._extractors = extractors

    def dts(self):
        return self._extractors._dts

    def edts(self):
        return self._extractors._edts

    def bindings(self):
        return self._extractors._bindings

    def extractors(self):
        return self._extractors

    ##
    # @brief Get EDTS device id associated to node address.
    #
    # @return ETDS device id
    def edts_device_id(self, node_address):
        return node_address


    def edts_insert_chosen(self, chosen, node_address):
        self.edts().insert_chosen(chosen, self.edts_device_id(node_address))

    ##
    # @brief Insert device property into EDTS
    #
    def edts_insert_device_property(self, node_address, property_path,
                                    property_value):

        if self.dts().get_node_compat(node_address) is None:
            # Node is a child of a device.
            # Get device ancestor
            parent_address = ''
            node_address_split = node_address.split('/')
            child_name = node_address_split[-1]
            for comp in node_address_split[1:-1]:
                parent_address += '/' + comp
            device_id = self.edts_device_id(parent_address)
            # Child is not a device, store it as a prop of his parent, under a child section
            # Use its relative address as parameter name since extraction can't work with '/'
            # inside property path.
            # An aboslute id will be computed and populated at injection
            self.edts().insert_child_property(device_id, child_name, property_path, property_value)
        else:
            device_id = self.edts_device_id(node_address)
            self.edts().insert_device_property(device_id, property_path, property_value)

    ##
    # @brief Insert device controller into EDTS
    #
    def edts_insert_device_controller(self, controller_address, node_address,
                                      line):
        device_id = self.edts_device_id(node_address)
        controller_id = self.edts_device_id(controller_address)
        controller_type = self.dts().get_controller_type(controller_address)
        self.edts().insert_device_controller(controller_type, controller_id,
                                            device_id, line)

    ##
    # @brief Insert device parent-device property into EDTS
    #
    def edts_insert_device_parent_device_property(self, node_address):
        # Check for a parent device this device is subordinated
        parent_device_id = None
        parent_node_address = ''
        for comp in node_address.split('/')[1:-1]:
            parent_node_address += '/' + comp
            compatibles = self.dts().reduced[parent_node_address]['props'] \
                                                            .get('compatible', None)
            if compatibles:
                # there is a parent device,
                # only use the ones that have a minimum control on the child
                if 'simple-bus' not in compatibles:
                    parent_device_id = self.edts_device_id(parent_node_address)
        if parent_device_id:
            device_id = self.edts_device_id(node_address)
            self.edts().insert_device_property(device_id, 'parent-device',
                                              parent_device_id)
    ##
    # @brief Extract directive information.
    #
    # @param node_address Address of node issuing the directive.
    # @param prop Directive property name
    #
    def extract(self, node_address, prop):
        pass
