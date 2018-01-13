# Copyright (c) 2017 Bobby Noelte <b0661n0e17e@gmail.com>
#
# SPDX-License-Identifier: Apache-2.0

# Prepare generic unit test environment
#
# parameters:
# - INCLUDE is relative to zephyr base
# - SOURCES defaults to main.c
#
# target:
# - testbinary (default)
list(APPEND INCLUDE subsys) # for ztest
include($ENV{ZEPHYR_BASE}/tests/unit/unittest.cmake)

# Fake zephyr environment to prepare the
# generation of include/generated/generated_dts_board.h from DTS
set(ZEPHYR_BASE $ENV{ZEPHYR_BASE})
set(PROJECT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(PROJECT_SOURCE_DIR ${ZEPHYR_BASE})
set(ARCH host)
set(BOARD_FAMILY host)
set(CONFIG_HAS_DTS True)
set(DTS_BINDINGS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bindings)
set(APPLICATION_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include(${ZEPHYR_BASE}/cmake/extensions.cmake)

# Get the tools for DTS - python and DTC
find_package(PythonInterp 3.4)
include(${ZEPHYR_BASE}/cmake/host-tools.cmake)

# Generate include/generated/generated_dts_board.h from DTS
set(AUTOCONF_H ${CMAKE_CURRENT_SOURCE_DIR}/autoconf.h)
set(DTS_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/test.dts)
include(${ZEPHYR_BASE}/cmake/dts.cmake)


# Make test aware of generated include file
target_include_directories(testbinary PRIVATE ${DTS_BINDINGS_DIR} ${CMAKE_CURRENT_BINARY_DIR}/include)

project(none) # switches PROJECT_SOURCE_DIR and PROJECT_BINARY_DIR
