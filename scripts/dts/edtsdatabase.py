#!/usr/bin/env python3
#
# Copyright (c) 2018 Bobby Noelte
# Copyright (c) 2018 Linaro Limited
#
# SPDX-License-Identifier: Apache-2.0
#

from pathlib import Path
import argparse
import pprint

from .edtsdb.database import EDTSDb
from .edtsdb.device import EDTSDevice

##
# @brief Extended DTS database
#
# Database schema:
#
# _edts dict(
#   'aliases': dict(alias) : sorted list(device-id)),
#   'chosen': dict(chosen),
#   'devices':  dict(device-id :  device-struct),
#   'compatibles':  dict(compatible : sorted list(device-id)),
#   'device-types': dict(device-type : sorted list(compatible)),
#   ...
# )
#
# device-struct dict(
#   'device-id' : device-id,
#   'compatible' : list(compatible) or compatible,
#   'label' : label,
#   property-name : property-value ...
# )
#
# Database types:
#
# device-id: opaque id for a device (do not use for other purposes),
# compatible: any of ['st,stm32-spi-fifo', ...] - 'compatibe' from <binding>.yaml
# label: any of ['UART_0', 'SPI_11', ...] - label directive from DTS
#
class EDTSDatabase(EDTSDb):

    def __init__(self, *args, **kw):
        super().__init__(*args, **kw)

    def main(self):
        args = self.parse_arguments()
        edts_file = Path(args.edts[0])
        if not edts_file.is_file():
            raise Exception(
                "Generated extended device tree database file '{}' not found/ no access.".
                format(edts_file))
        self.load(str(edts_file))

        pprint.pprint(dict(self._edts))

        return 0


if __name__ == '__main__':
    EDTSDatabase().main()
