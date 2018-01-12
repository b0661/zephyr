#
# Copyright (c) 2017 Bobby Noelte
#
# SPDX-License-Identifier: Apache-2.0
#

##
# @brief Base class for device tree directives
#
class DTDirective(object):

    def __init__():
        pass

    ##
    # @brief Extract directive information.
    #
    # @param node_address Address of node issueuing the directive.
    # @param yaml YAML definition for the node.
    # @param y_key Directive key
    # @param names Names assigned to directive.
    # @param[out] defs Property definitions for each node address
    # @param def_label Define label string of node owning the directive.
    #
    def extract(self, node_address, yaml, y_key, names, defs, def_label):
        pass
