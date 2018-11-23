cmake_minimum_required(VERSION 3.9)

set(BOARD_ROOT ${CMAKE_SOURCE_DIR})
set(BOARD refboard)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)

target_sources(app PRIVATE ${ZEPHYR_BASE}/misc/empty_file.c)

# Keep last
project(test_edts)
