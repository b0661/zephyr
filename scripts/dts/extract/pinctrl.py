#
# Copyright (c) 2017 Bobby Noelte
#
# SPDX-License-Identifier: Apache-2.0
#

from extract.globals import *
from extract.directive import DTDirective

##
# @brief Manage pinctrl-x directive.
#
class DTPinCtrl(DTDirective):

    ##
    # @brief Boolean type properties for pin configuration by pinctrl.
    conf_bool_props = [
        "bias-disable", "bias-high-impedance", "bias-bus-hold",
        "drive-push-pull", "drive-open-drain", "drive-open-source",
        "input-enable", "input-disable", "input-schmitt-enable",
        "input-schmitt-disable", "low-power-enable", "low-power-disable",
        "output-disable", "output-enable", "output-low","output-high"]
    ##
    # @brief Boolean or value type properties for pin configuration by pinctrl.
    conf_bool_or_value_props = [
        "bias-pull-up", "bias-pull-down", "bias-pull-pin-default"]
    ##
    # @brief List type properties for pin configuration by pinctrl.
    conf_list_props = [
        "pinmux", "pins", "group", "drive-strength", "input-debounce",
        "power-source", "slew-rate", "speed"]

    def __init__(self):
        self._data = {}
        ##
        # @brief Number of functions in function dataset of pin controllers.
        self._function_count = {}
        ##
        # @brief Number of pinctrl state names in state name dataset of
        #        pin controllers.
        self._state_name_count = {}
        ##
        # @brief Number of pinctrl states in state dataset of pin controllers.
        self._state_count = {}
        ##
        # @brief Number of pinctrls in pinctrl dataset of pin controllers.
        self._pinctrl_count = {}
        ##
        # @brief Number of pin controller info in pin controller dataset
        #        of clients (nodes that issued a pinctrl-x directive).
        self._pin_controller_count = {}

    ##
    # @brief Get pin controller's pinctrl label prefix for pinctrl
    #
    # @param pinctrl_id Unique identifier of a specific pinctrl portion
    #
    def _get_pinctrl_label_prefix(self, pinctrl_id):
        return self._data.get(pinctrl_id, None)

    ##
    # @brief Map pin controller's pinctrl label prefix to pinctrl
    #
    # @param pinctrl_id Unique identifier ofa specific pinctrl portion
    #
    def _map_pinctrl_label_prefix(self, pinctrl_id, pinctrl_label_prefix):
        self._data[pinctrl_id]  = pinctrl_label_prefix
        return

    ##
    # @brief Get the function index within dataset of pin controller.
    #
    # @param pin_controller Label string of pin controller.
    # @param function Label string denoting the node (aka. function).
    # @retval -1 if there is no function info available.
    # @returns the function index
    #
    def _get_function_index(self, pin_controller, function):
        label = pin_controller + function
        return self._data.get(label, -1)

    ##
    # @brief Map the index of function within dataset of pin controller.
    #
    # The function is mapped to the node.
    #
    # @param pin_controller Label string of pin controller.
    # @param function Label string denoting the node (aka. function).
    # @param function_index the function index
    #
    def _map_function_index(self, pin_controller, function, function_index):
        label = pin_controller + function
        self._data[label]  = function_index
        return

    ##
    # @brief Get the state name index within dataset of pin controller.
    #
    # @param pin_controller Label string of pin controller.
    # @param state_name State name.
    # @retval -1 if there is no state name info available.
    # @returns the state name index
    #
    def _get_state_name_index(self, pin_controller, state_name):
        label = pin_controller + state_name
        return self._data.get(label, -1)

    ##
    # @brief Map the index of state name within dataset of pin controller.
    #
    # @param pin_controller Label string of pin controller.
    # @param state_name State name.
    # @param state_name_index the state name index
    #
    def _map_state_name_index(self, pin_controller, state_name, state_name_index):
        label = pin_controller + state_name
        self._data[label]  = state_name_index
        return

    ##
    # @brief Get the index of pinctrl state within dataset of pin controller.
    #
    # @param pin_controller Label string of pin controller.
    # @param owner Label string denoting the state owner.
    # @param state_name_index Index of state name of the pinctrl state.
    # @retval -1 if there is no state available.
    # @returns the state index
    #
    def _get_state_index(self, pin_controller, owner, state_name_index):
        label = pin_controller + owner + str(state_name_index)
        return self._data.get(label, -1)

    ##
    # @brief Map the index of pinctrl state within dataset of pin controller.
    #
    # The state is mapped to the node that owns the pinctrl state with the
    # given pinctrl name.
    #
    # @param pin_controller Label string of pin controller.
    # @param owner Label string denoting the state owner
    # @param state_name_index Index of state name of the pinctrl state.
    # @param state_index the state index
    #
    def _map_state_index(self, pin_controller, owner, state_name_index, state_index):
        label = pin_controller + owner + str(state_name_index)
        self._data[label]  = state_index
        return

    ##
    # @brief Get the index of pin controller info within dataset of client.
    #
    # @param owner Label string denoting the pin controller client.
    # @param pin_controller Label string of pin controller.
    # @retval -1 if there is no state available.
    # @returns the pin controller index
    #
    def _get_pin_controller_index(self, owner, pin_controller):
        label = owner + pin_controller
        return self._data.get(label, -1)

    ##
    # @brief Map the index of pin controller info within dataset of client.
    #
    # The pin controller is mapped to the node (client) that references
    # the pin controller.
    #
    # @param owner Label string denoting the pin controller client.
    # @param pin_controller Label string of pin controller.
    # @param pin_controller_index pin controller index
    #
    def _map_pin_controller_index(self, owner, pin_controller, pin_controller_index):
        label = owner + pin_controller
        self._data[label]  = pin_controller_index
        return

    ##
    # @brief Initialise function info for the pin's pinctrl info
    #
    # @param client_label Label of node owning the pin.
    # @param pin_controller_label Label of pin controller that controls the pin.
    # @param[out] prop_def Pin controller properties that will be converted
    #                      to defines.
    # @return index of function info within pin controllers functions dataset
    #
    def _init_function_info(self, client_label, pin_controller_label,
                            prop_def):
        index = self._get_function_index(pin_controller_label,
                                                  client_label)
        if index < 0:
            # Function is not mapped
            # Map function to new pin controller function info
            index = self._function_count.get(pin_controller_label, 0)
            self._function_count[pin_controller_label] = index + 1
            self._map_function_index(pin_controller_label,
                                     client_label, index)

            # Init/update the property describing the total count of functions
            # of this specific pin controller
            index_label = get_label_string([pin_controller_label,
                                            "FUNCTION", "COUNT"])
            prop_def[index_label] = self._function_count[pin_controller_label]

            # Set properties describing the function
            function_label = get_label_string([pin_controller_label,
                                "FUNCTION", str(index), "CLIENT"])
            prop_def[function_label] = client_label
        return index

    ##
    # @brief Initialise state name info for the pin's pinctrl info
    #
    # @param pin_controller_label Label of pin controller.
    # @param pinctrl_name Name assigned to pinctrl state
    # @param[out] prop_def Pin controller properties that will be converted
    #                      to defines.
    # @return index of state name info within pin controllers state name dataset
    #
    def _init_state_name_info(self, pin_controller_label, pinctrl_name,
                                    prop_def):
        index = self._get_state_name_index(pin_controller_label, pinctrl_name)
        if index < 0:
            # State name is not mapped
            # Map state name to new pin controller state name info
            index = self._state_name_count.get(pin_controller_label, 0)
            self._state_name_count[pin_controller_label] = index + 1
            self._map_state_name_index(pin_controller_label, pinctrl_name,
                                       index)

            # Init/update the property describing the total count of functions
            # of this specific pin controller
            index_label = get_label_string([pin_controller_label,
                                            "STATE", "NAME", "COUNT"])
            prop_def[index_label] = self._state_name_count[ \
                                                        pin_controller_label]

            # Set properties describing the state name
            label = get_label_string([pin_controller_label,
                                                "STATE", "NAME", str(index)])
            prop_def[label] = pinctrl_name
        return index

    ##
    # @brief Initialise pinctrl state info for the pin's pinctrl info
    #
    # Pinctrl state info references the function index of the function the
    # state applies to.
    #
    # @param client_label Label of node owning the pin.
    # @param pin_controller_label Label of pin controller that controls the
    #                             pin.
    # @param state_name_index Index of pinctrl state name info within pin
    #                         controllers state name dataset.
    # @param function_index Index of function info within pin controllers
    #                       functions dataset.
    # @param[out] prop_def Pin controller properties that will be converted
    #                      to defines.
    # @return index of state info within pin controllers states dataset
    #
    def _init_state_info(self, client_label, pin_controller_label,
                         state_name_index, function_index, prop_def):
        index = self._get_state_index(pin_controller_label,
                                            client_label, state_name_index)
        if index < 0:
            # State is not mapped
            # Map owner state to new pin controller state
            index = self._state_count.get(pin_controller_label, 0)
            self._state_count[pin_controller_label] = index + 1
            self._map_state_index(pin_controller_label,
                                  client_label, state_name_index, index)

            # Init/update the property describing the total count of pinctrl states
            # of this specific pin controller
            index_label = get_label_string([pin_controller_label,
                                "PINCTRL", "STATE", "COUNT"])
            prop_def[index_label] = self._state_count[pin_controller_label]

            # Set properties describing the pinctrl state
            state_label = get_label_string([pin_controller_label,
                            "PINCTRL", "STATE", str(index), "NAME_ID"])
            prop_def[state_label] = state_name_index
            state_label = get_label_string([pin_controller_label,
                            "PINCTRL", "STATE", str(index), "FUNCTION_ID"])
            prop_def[state_label] = function_index
        return index

    ##
    # @brief Initialise pinctrl info of pin controller
    #
    # Pinctrl info references the pinctrl state index of the state the
    # pinctrl info belongs to.
    #
    # @param pin_controller_label Label of pin controller that controls the
    #                             pin.
    # @param state_index Index of state info within pin controllers
    #                       pinctrl states dataset.
    # @param[out] prop_def Pin controller properties that will be converted
    #                      to defines.
    # @return pinctrl label prefix
    #
    def _init_pinctrl_info(self, pin_controller_label, state_index, prop_def):
        index = self._pinctrl_count.get(pin_controller_label, 0)
        self._pinctrl_count[pin_controller_label] = index + 1

        # Init/update the property describing the total count of pinctrl info
        # of this specific pin controller
        index_label = get_label_string([pin_controller_label, \
                                                        "PINCTRL", "COUNT"])
        prop_def[index_label] = self._pinctrl_count[pin_controller_label]

        # create initial pin properties (all 0)
        # will be overridden by "real" configuration
        pinctrl_label_prefix = [pin_controller_label, "PINCTRL", str(index)]
        for cells in (self.conf_bool_props +
                      self.conf_bool_or_value_props +
                      self.conf_list_props):
            pinctrl_label = get_label_string(pinctrl_label_prefix + [cells])
            prop_def[pinctrl_label] = 0

        # Set properties describing the pinctrl info
        pinctrl_label = get_label_string(pinctrl_label_prefix + ['STATE_ID'])
        prop_def[pinctrl_label] = state_index

        return get_label_string(pinctrl_label_prefix)


    ##
    # @brief Initialise pin controller info of the client.
    #
    # @param client_label Label of node owning the pin.
    # @param pin_controller_label Label of pin controller that controls the
    #                             pin.
    # @param function_index Index of function info within pin controllers
    #                       functions dataset.
    # @param[out] prop_def Client properties that will be converted
    #                      to defines.
    # @return Index of pin controller info within client's pin controller
    #         dataset.
    #
    def _init_pin_controller_info(self, client_label, pin_controller_label,
                                        function_index, prop_def):
        index = self._get_pin_controller_index(client_label,
                                               pin_controller_label)

        if index < 0:
            # Pin controller is not mapped
            # Map pin controller to new pin controller info of pin owner
            index = self._pin_controller_count.get(client_label, 0)
            self._pin_controller_count[pin_controller_label] = index + 1
            self._map_pin_controller_index(client_label,
                                           pin_controller_label, index)

            # Init/update the property describing the total count of
            # pin controller info of this specific pin owner node
            index_label = get_label_string([client_label,
                                "PINCTRL", "CONTROLLER", "COUNT"])
            prop_def[index_label] = \
                                self._pin_controller_count[pin_controller_label]

            # Set properties describing the pin controller
            label = get_label_string([client_label,
                            "PINCTRL", "CONTROLLER", str(index), "CONTROLLER"])
            prop_def[label] = pin_controller_label
            label = get_label_string([client_label,
                            "PINCTRL", "CONTROLLER", str(index), "FUNCTION_ID"])
            prop_def[label] = function_index
        return index


    ##
    # @brief Initialise pinctrl information of pin.
    #
    # Initialise/ update the pinctrl management information.
    # - Index of initialisation info for the pin of the specific pin controller.
    #   <pin_controller_label>_PINCTRL_COUNT: <index>
    # - Mapping from pin pinctrl identifier to pin default label string.
    #   <pin_pinctrl_id>: <pin_controller_base_label>_PINCTRL_<index>
    # - Mapping of pinctrl name of owner node to pinctrl state index
    #   <client_label>_<pinctrl_name>: <state_index>
    # - Mapping of owner node to pinctrl function index
    #   <client_label>: <function_index>
    #
    # Initialise the pin's pinctrl properties in the pin controller's
    # properties definition. Four datasets are initialised:
    # - FUNCTION
    #   - <pin_controller_label>_FUNCTION_<index>_FUNCTION:
    #       <client_label>
    #   - <pin_controller_label>_FUNCTION_<index>_LABEL:
    #       <client_label>_LABEL
    #   - <pin_controller_label>_FUNCTION_<index>_BASE_ADDRESS:
    #       <client_label>_BASE_ADDRESS
    #   - <pin_controller_label>_FUNCTION_COUNT: <index> + 1
    # - STATE_NAME
    #   - <pin_controller_label>_STATE_NAME_<index>: <pinctrl_name>
    #   - <pin_controller_label>_STATE_NAME_COUNT: <index> + 1
    # - PINCTRL_STATE
    #   - <pin_controller_label>_PINCTRL_STATE_<index>_NAME_ID:
    #       <STATE_NAME index>
    #   - <pin_controller_label>_PINCTRL_STATE_<index>_FUNCTION_ID:
    #       <FUNCTION index>
    #   - <pin_controller_label>_PINCTRL_STATE_COUNT: <index> + 1
    # - PINCTRL
    #   - <pin_controller_label>_PINCTRL_<index>_<property>: 0
    #   - <pin_controller_label>_PINCTRL_<index>_STATE: <PINCTRL_STATE index>
    #   - <pin_controller_label>_PINCTRL_COUNT: <index> + 1
    #
    # Initialise the pin's pinctrl properties in the owner (client) node's
    # properties definition:
    # - PINCTRL_CONTROLLER
    #   - <client_label>_PINCTRL_CONTROLLER_<index>_FUNCTION:
    #       <FUNCTION index>
    #   - <client_label>_PINCTRL_CONTROLLER_<index>_LABEL:
    #       <pin_controller_label>_LABEL
    #   - <client_label>_PINCTRL_CONTROLLER_<index>_BASE_ADDRESS:
    #       <pin_controller_label>_BASE_ADDRESS
    #   - <client_label>_PINCTRL_CONTROLLER_COUNT: <index> + 1
    # - PINCTRL_<pinctrl_name>
    #   - <client_label>_PINCTRL_<pinctrl_name>_CONTROLLER:
    #       <PINCTRL_CONTROLLER index>
    #   - <client_label>_PINCTRL_<pinctrl_name>_STATE:
    #       <PINCTRL_STATE index>
    #
    # The function assures that the initialisation is only done once.
    #
    # @param pinctrl_id Unique identifier for a specific pinctrl portion
    # @param client_label Label of node issueing the pinctrl directive.
    # @param pin_controller_label Label string of pin controller controlling the
    #                             pin.
    # @param pinctrl_name Name assigned to pinctrl pinctrl-<index>.
    # @param[out] client_prop_def Pin controller client node properties that will
    #                            be converted to defines.
    # @param[out] pin_controller_prop_def Pin controller properties that will
    #                                     be converted to defines.
    #
    def _init_pinctrl(self, pinctrl_id, client_label, pin_controller_label,
                            pinctrl_name,
                            client_prop_def, pin_controller_prop_def):

        if self._get_pinctrl_label_prefix(pinctrl_id) is not None:
            # Already initialized
            return

        # Set/ get the pin controller's function info
        pin_controller_function_index = self._init_function_info(
                                                client_label,
                                                pin_controller_label,
                                                pin_controller_prop_def)

        # Set/ get the pin controller's pinctrl state name info
        pin_controller_state_name_index = self._init_state_name_info(
                                                pin_controller_label,
                                                pinctrl_name,
                                                pin_controller_prop_def)

        # Set/ get the pin controller's pinctrl state info
        pin_controller_state_index = self._init_state_info(client_label,
                                                pin_controller_label,
                                                pin_controller_state_name_index,
                                                pin_controller_function_index,
                                                pin_controller_prop_def)

        # Set the pin controller's pinctrl info to initial values
        pinctrl_label_prefix = self._init_pinctrl_info(
                                                pin_controller_label,
                                                pin_controller_state_index,
                                                pin_controller_prop_def)

        # Set the owner's (client) pin controller info
        client_pin_controller_index = self._init_pin_controller_info(
                                                client_label,
                                                pin_controller_label,
                                                pin_controller_function_index,
                                                client_prop_def)

        # Set the owner's (client) pinctrl info
        pinconf_label = get_label_string( \
                    [client_label, "PINCTRL", pinctrl_name, "CONTROLLER_ID"])
        client_prop_def[pinconf_label] = client_pin_controller_index
        pinconf_label = get_label_string( \
                    [client_label, "PINCTRL", pinctrl_name, "STATE_ID"])
        client_prop_def[pinconf_label] = pin_controller_state_index

        # remember pin controller's pinctrl label prefix associated
        # to this pinctrl portion
        self._map_pinctrl_label_prefix(pinctrl_id, pinctrl_label_prefix)
        return

    ##
    # @brief Extract pinctrl information.
    #
    # Pinctrl information is added to the client node that owns the pinctrl
    # directive
    # - <client_label>_PINCTRL_<index>_<property>: <property value>
    #
    # and to the pin controller node that controls the pin:
    # - <pin_controller_label>_PINCTRL_<index>_<property>: <property value>
    #
    # @param node_address Address of node owning the pinctrl definition.
    # @param yaml YAML definition for the owning node.
    # @param y_key pinctrl-x key
    # @param names Names assigned to pinctrl state pinctrl-<index>.
    # @param[out] defs Property definitions for each node address
    # @param def_label Define label string of client node owning the pinctrl
    #                  definition.
    #
    def extract(self, node_address, yaml, y_key, names, defs, def_label):

        # Get pinctrl index from pinctrl-<index> directive
        pinctrl_index = int(y_key.split('-')[1])
        # Pinctrl definition
        pinctrl = reduced[node_address]['props'][y_key]
        # Name of this pinctrl state. Use directive if there is no name.
        if pinctrl_index >= len(names):
            pinctrl_name = y_key
        else:
            pinctrl_name = names[pinctrl_index]

        pin_config_handles = []
        if not isinstance(pinctrl, list):
            pin_config_handles.append(pinctrl)
        else:
            pin_config_handles = list(pinctrl)

        client_prop_def = {}
        for pin_config_handle in pin_config_handles:
            pin_config_node_address = phandles[pin_config_handle]
            pin_config_name = get_label_string( \
                                    pin_config_node_address.split('/')[-1:])
            pin_config_subnode_prefix = \
                            '/'.join(pin_config_node_address.split('/')[-1:])
            pin_controller_node_address = \
                            '/'.join(pin_config_node_address.split('/')[:-1])
            pin_controller_label = get_node_label_string( \
                                                pin_controller_node_address)
            pin_controller_prop_def = {}
            for subnode in reduced.keys():
                if pin_config_subnode_prefix in subnode \
                    and pin_config_node_address != subnode:
                    # found a subnode underneath the pin configuration node
                    pin_config_subnode_name = ''.join(subnode.split('/')[-1:])
                    # Create an identifier for this specific pinctrl portion.
                    pinctrl_id = def_label + pinctrl_name \
                                 + pin_controller_label + pin_config_name \
                                 + pin_config_subnode_name
                    # create pin control directives
                    for i, cells in enumerate(reduced[subnode]['props']):
                        pinconf = reduced[subnode]['props'][cells]
                        if cells in self.conf_bool_props:
                            self._init_pinctrl(pinctrl_id, def_label, \
                                pin_controller_label, pinctrl_name, \
                                client_prop_def, pin_controller_prop_def)
                            pinconf = 1 if pinconf else 0
                        elif cells in self.conf_bool_or_value_props:
                            self._init_pinctrl(pinctrl_id, def_label, \
                                pin_controller_label, pinctrl_name, \
                                client_prop_def, pin_controller_prop_def)
                            if isinstance(pinconf, bool):
                                pinconf = 1 if pinconf else 0
                        elif cells in self.conf_list_props:
                            self._init_pinctrl(pinctrl_id, def_label, \
                                pin_controller_label, pinctrl_name, \
                                client_prop_def, pin_controller_prop_def)
                            if isinstance(pinconf, list):
                                pinconf = ','.join(map(str, pinconf))
                        else:
                            # unknown pin configuration property name
                            # use cell names if available
                            # (for a pinmux node these are: pin, function)
                            parent_yaml = yaml[get_compat(reduced[ \
                                                    pin_controller_node_address])]
                            cell_prefix = parent_yaml.get('cell_string', None)
                            cell_names = parent_yaml.get('#cells', None)
                            post_fix = []
                            if cell_prefix is not None:
                                post_fix.append(cell_prefix)
                            if cell_names is None:
                                # No cell names - use default names for pinctrl
                                cell_names = ['pin', 'function']
                            key_label = get_label_string([def_label] \
                                        + post_fix + subnode.split('/')[-2:] \
                                        + [cell_names[0], str(i)])
                            func_label = get_label_string([def_label] \
                                        + post_fix + subnode.split('/')[-2:] \
                                        + [cell_names[1], str(i)])
                            client_prop_def[key_label] = cells
                            client_prop_def[func_label] = pinconf
                            # unknown -> skip pin controller property update
                            continue
                        # update client pinctrl property definition
                        label = get_label_string([def_label, "PINCTRL", \
                                pinctrl_name, pin_config_subnode_name, cells])
                        client_prop_def[label] = pinconf
                        # update pin controller pinctrl property definition
                        label_prefix = self._get_pinctrl_label_prefix( \
                                                                    pinctrl_id)
                        label = get_label_string([label_prefix, cells])
                        pin_controller_prop_def[label] = pinconf
            # update property definitions of related pin controller node
            insert_defs(pin_controller_node_address, defs, pin_controller_prop_def, {})
        # update property definitions of owning node
        insert_defs(node_address, defs, client_prop_def, {})
        return


