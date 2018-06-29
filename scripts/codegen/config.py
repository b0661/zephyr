# Copyright (c) 2018 Bobby Noelte.
#
# SPDX-License-Identifier: Apache-2.0

import re

class ConfigMixin(object):
    def __init__(self, path_to_kconfig_fragment):
        self.__slots__ = []

        self._kconfig_fragment_as_dict = self.__construct_dict_from_path(
            path_to_kconfig_fragment
        )

    ##
    # @brief Get all config properties.
    #
    # The property names are from the Kconfig fragment .config.
    #
    # @return A dictionary of config properties.
    #
    def config_properties(self):
        return dict(self._kconfig_fragment_as_dict)


    def __construct_dict_from_path(self, path):
        with open(str(path)) as fd:
            relevant_lines = [line for line in fd if line.startswith('CONFIG_')]
            for line in relevant_lines:
                # CONFIG looks like: CONFIG_NET_BUF=y

                symbol, value = line.rstrip().split(sep= '=', maxsplit=1)

                typed_value = self.__convert_string_value_to_typed_value(value)

                yield symbol, typed_value

    def __convert_string_value_to_typed_value(self, value):
        is_string = value.startswith('"')
        is_bool = value == 'y' or value == 'n' # =n does not occur in Kconfig fragments yet (#5443)
        is_hex = value.startswith('0x')

        if is_string:
            return value[1:-1] # Cut of prefix and suffix '"'
        elif is_bool:
            return value == 'y'
        elif is_hex:
            return int(value[2:], 16)
        else:
            # Assume that the only types that exist are the above and
            # 'int'
            return int(value)
