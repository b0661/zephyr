#
# Copyright (c) 2018 Bobby Noelte
#
# SPDX-License-Identifier: Apache-2.0
#

from .directive import DTDirective

##
# @brief Manage flash directives.
#
class DTFlash(DTDirective):

    def __init__(self, extractors):
        super().__init__(extractors)
        # node addresses of flash nodes already seen
        self._flash_node_addresses = []

    #
    # @returns partition name
    def _insert_partition(self, node_address, nr_address_cells, nr_size_cells):
        dts = self.dts()

        node = dts.reduced[node_address]
        partition_name = node['props']['label']
        partition_sectors = node['props']['reg']

        # propate size info from partitions
        self.edts_insert_device_property(node_address, '#address-cells',
                                         nr_address_cells)
        self.edts_insert_device_property(node_address, '#size-cells',
                                         nr_size_cells)
        # generate sectors
        sector_index = 0
        sector_cell_index = 0
        sector_nr_cells = nr_address_cells + nr_size_cells
        for cell in partition_sectors:
            if sector_cell_index < nr_address_cells:
               self.edts_insert_device_property(node_address,
                    'sector/{}/offset/{}'.format(sector_index, sector_cell_index),
                    cell)
            else:
                size_cell_index = sector_cell_index - nr_address_cells
                self.edts_insert_device_property(node_address,
                    'sector/{}/size/{}'.format(sector_index, size_cell_index),
                    cell)
            sector_cell_index += 1
            if sector_cell_index >= sector_nr_cells:
                sector_cell_index = 0
                sector_index += 1
        return partition_name

    def _extract_partitions(self, node_address, prop):
        dts = self.dts()
        edts = self.edts()
        extractors = self.extractors()

        if dts.get_compat(node_address) != 'fixed-partitions':
            raise Exception(
                "DTFlash.extract called with unexpected compatible"
                " ({}) for partitions - fixed-partions expected."
                .format(dts.get_compat(node_address)))

        # This is a partitions node; look for the flash it belongs to
        flash_node_address = self.dts().get_parent_address(node_address)
        # Do a minimal check on flash node
        if not dts.get_node_compat(flash_node_address):
            raise Exception(
                "Flash node '{}' without compatible definition."
                    .format(flash_node_address))
        if flash_node_address not in self._flash_node_addresses:
            # we have not seen this flash node before
            self._extract_flash(flash_node_address, prop)

        # add partitions to flash device
        flash_device_id = self.edts_device_id(flash_node_address)
        nr_address_cells = dts.reduced[node_address]['props'].get('#address-cells')
        nr_size_cells = dts.reduced[node_address]['props'].get('#size-cells')

        partition_adresses = dts.get_children_addresses(node_address)
        for partition_node_address in partition_adresses:
            partition_name = self._insert_partition(partition_node_address,
                                                    nr_address_cells, nr_size_cells)
            edts.insert_device_property(flash_device_id,
                                        'partitions/{}'.format(partition_name),
                                        partition_node_address)

    def _extract_flash(self, node_address, prop):
        dts = self.dts()
        edts = self.edts()
        extractors = self.extractors()

        device_id = self.edts_device_id(node_address)

        if node_address == 'dummy-flash':
            # We will add addr/size of 0 for systems with no flash controller
            # This is what they already do in the Kconfig options anyway
            edts.insert_device_property(device_id, 'reg/0/address/0', "0")
            edts.insert_device_property(device_id, 'reg/0/size/0', "0")
        else:
            self._flash_node_addresses.append(node_address)

            flash_props = ["label", "write-block-size", "erase-block-size"]
            for prop in flash_props:
                if prop in  dts.reduced[node_address]['props']:
                    extractors.default.extract(node_address, prop)

            # add flash to flash controller
            flash_controller_address = dts.get_parent_address(node_address)
            flash_controller_device_id = self.edts_device_id(flash_controller_address)
            flash_name = dts.reduced[node_address]['props']['label']
            edts.insert_device_property(flash_controller_device_id,
                                        'flash-devices/{}'.format(flash_name),
                                        node_address)

            # add flash controller to flash
            edts.insert_device_property(device_id, 'controller',
                                        flash_controller_address)

    def _extract_code_partition(self, node_address, prop):
        # do sanity check
        if node_address != 'dummy-flash':
            if len(self._flash_node_addresses) == 0:
                # No flash node scanned before-
                raise Exception(
                    "Code partition '{}' {} without flash definition."
                        .format(prop, node_address))

    ##
    # @brief Extract flash
    #
    # @param node_address Address of node issuing the directive.
    # @param prop Directive property name
    #
    def extract(self, node_address, prop):

        if node_address.endswith('/partitions'):
            # indicator for partitions
            if prop in ("#address-cells", "#size-cells"):
                # covered by compatible
                pass
            elif prop == 'compatible':
                self._extract_partitions(node_address, prop)
            else:
                raise Exception(
                    "DTFlash.extract called with unexpected directive"
                    " ({}) for partitions.".format(prop))
        elif prop == 'zephyr,flash':
            # indicator for flash
            self._extract_flash(node_address, prop)
        elif prop == 'zephyr,code-partition':
            # indicator for code_partition
            self._extract_code_partition(node_address, prop)
        else:
            raise Exception(
                "DTFlash.extract called with unexpected directive ({})."
                    .format(prop))
