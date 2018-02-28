# Copyright (c) 2018 Bobby Noelte.
#
# SPDX-License-Identifier: Apache-2.0

from pathlib import Path
from dts.edtsdatabase import EDTSDatabase
from .filelock import Timeout, FileLock

class EDTSMixin(object):
    __slots__ = []

    _edts = None

    def _edts_assure(self):
        if self._edts is not None:
            return

        # EDTS database file
        # If not given '<codegen>' is used to indicate that the EDTS database
        # shall be extracted.
        edts_file = self.cmake_variable("GENERATED_EDTS", "<codegen>")

        # EDTS database file lock
        app_bin_dir = self.cmake_variable("APPLICATION_BINARY_DIR")
        edts_lock_file = Path(app_bin_dir, 'codegen_edts.lock')
        edts_lock = FileLock(edts_lock_file, timeout=1)

        # Try to get a lock for the database file
        # If we do not get the log for 10 seconds an
        # exception is thrown.
        try:
            with edts_lock.acquire(timeout = 10):
                if edts_file == "<codegen>":
                    # EDTS database file
                    edts_file = Path(app_bin_dir, 'codegen_edts.json')
                    # File produced by the DT compiler
                    board = self.cmake_variable("BOARD")
                    prj_bin_dir = self.cmake_variable("PROJECT_BINARY_DIR")
                    dts_compiled = Path(prj_bin_dir, '{}.dts_compiled'.format(board))

                    if edts_file.is_file():
                        # file must be newer than the DTS compiler file
                        dts_date = dts_compiled.stat().st_mtime
                        edts_date = edts_file.stat().st_mtime
                        if dts_date > edts_date:
                            # Remove old file
                            edts_file.unlink()

                    if not edts_file.is_file():
                        # Create list of bindings directories
                        app_bindings_dir = self.cmake_variable("DTS_APP_BINDINGS")
                        bindings_dirs = []
                        bindings_dirs.append(str(Path(self.zephyr_path(), 'dts/bindings')))
                        bindings_dirs.append(str(Path(app_bindings_dir)))

                        # Create EDTS database by extraction
                        self._edts = EDTSDatabase()
                        self._edts.extract(dts_compiled, bindings_dirs)
                        # Store file to be reused
                        self._edts.save(str(edts_file))
                        return
                else:
                    edts_file = Path(edts_file)
                    if not edts_file.is_file():
                        raise self._get_error_exception(
                            "Generated extended device tree database file '{}' not found/ no access.".
                            format(edts_file), 2)

                self._edts = EDTSDatabase()
                self._edts.load(str(edts_file))

        except Timeout:
            # Something went really wrong - we did not get the lock
            raise self._get_error_exception(
                "Generated extended device tree database file '{}' no access.".
                format(edts_file), 2)

    ##
    # @brief Get the extended device tree database.
    #
    # @return Extended device tree database.
    #
    def edts(self):
        self._edts_assure()
        return self._edts
