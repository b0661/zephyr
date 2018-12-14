# Copyright (c) 2018 Bobby Noelte.
#
# SPDX-License-Identifier: Apache-2.0

find_program(COGENO_EXECUTABLE cogeno)

if(EXISTS "${COGENO_EXECUTABLE}")
    # We do not need the Python 3 interpreter
    set(COGENO_EXECUTABLE_OPTION)
else()
    # Cogeno is not installed.
    # Maybe the cogeno repository was cloned
    # or the Zephyr copy is available.
    find_file(COGENO_PY cogeno.py
              PATHS $ENV{HOME}/cogeno/cogeno
                    $ENV{ZEPHYR_BASE}/../cogeno/cogeno
                    $ENV{ZEPHYR_BASE}/scripts/cogeno/cogeno)

    if(NOT EXISTS "${COGENO_PY}")
        message(FATAL_ERROR "Cogeno not found - '${COGENO_PY}'.")
    endif()

    if(${CMAKE_VERSION} VERSION_LESS "3.12")
        set(Python_ADDITIONAL_VERSIONS 3.7 3.6 3.5)
        find_package(PythonInterp)

        set(Python3_Interpreter_FOUND ${PYTHONINTERP_FOUND})
        set(Python3_EXECUTABLE ${PYTHON_EXECUTABLE})
        set(Python3_VERSION ${PYTHON_VERSION_STRING})
    else()
        # CMake >= 3.12
        find_package(Python3 COMPONENTS Interpreter)
    endif()

    if(NOT ${Python3_Interpreter_FOUND})
        message(FATAL_ERROR "Python 3 not found")
    endif()

    set(COGENO_EXECUTABLE "${Python3_EXECUTABLE}")
    set(COGENO_EXECUTABLE_OPTION "${COGENO_PY}")

    message(STATUS "Found cogeno: '${COGENO_PY}'.")
endif()

# --config
if(COGENO_CONFIG)
    set(COGENO_CONFIG_OPTION "--config" "${COGENO_CONFIG}")
else()
    set(COGENO_CONFIG_OPTION)
endif()

# --dts
# --bindings
# --edts
if(COGENO_DTS)
    set(COGENO_DTS_OPTION "--dts" "${COGENO_DTS}")
    if(COGENO_BINDINGS)
        set(COGENO_BINDINGS_OPTION "--bindings" ${COGENO_BINDINGS})
    else()
        set(COGENO_BINDINGS_OPTION)
    endif()
    if(COGENO_EDTS)
        set(COGENO_EDTS_OPTION "--edts" "${COGENO_EDTS}")
    else()
        set(COGENO_EDTS_OPTION "--edts" "${CMAKE_BINARY_DIR}/edts.json")
    endif()
else()
    set(COGENO_DTS_OPTION)
    set(COGENO_BINDINGS_OPTION)
    if(COGENO_EDTS)
        set(COGENO_EDTS_OPTION "--edts" "${COGENO_EDTS}")
    else()
        set(COGENO_EDTS_OPTION)
    endif()
endif()

# --modules
if(COGENO_MODULES)
    set(COGENO_MODULES_OPTION "--modules" ${COGENO_MODULES})
else()
    set(COGENO_MODULES_OPTION)
endif()

# --modules
if(COGENO_TEMPLATES)
    set(COGENO_TEMPLATES_OPTION "--templates" ${COGENO_TEMPLATES})
else()
    set(COGENO_TEMPLATES_OPTION)
endif()

function(target_sources_cogeno
    target          # The CMake target that depends on the generated file
    )

    set(options)
    set(oneValueArgs)
    set(multiValueArgs COGENO_DEFINES DEPENDS)
    cmake_parse_arguments(COGENO "${options}" "${oneValueArgs}"
                          "${multiValueArgs}" ${ARGN})
    # prepend -D to all defines
    string(REGEX REPLACE "([^;]+)" "-D;\\1"
            COGENO_COGENO_DEFINES "${COGENO_COGENO_DEFINES}")

    message(STATUS "Will generate for target ${target}")
    # Generated file must be generated to the current binary directory.
    # Otherwise this would trigger CMake issue #14633:
    # https://gitlab.kitware.com/cmake/cmake/issues/14633
    foreach(arg ${COGENO_UNPARSED_ARGUMENTS})
        if(IS_ABSOLUTE ${arg})
            set(template_file ${arg})
            get_filename_component(generated_file_name ${arg} NAME)
            set(generated_file ${CMAKE_CURRENT_BINARY_DIR}/${generated_file_name})
        else()
            set(template_file ${CMAKE_CURRENT_SOURCE_DIR}/${arg})
            set(generated_file ${CMAKE_CURRENT_BINARY_DIR}/${arg})
        endif()
        get_filename_component(template_dir ${template_file} DIRECTORY)
        get_filename_component(generated_dir ${generated_file} DIRECTORY)

        if(IS_DIRECTORY ${template_file})
            message(FATAL_ERROR "target_sources_cogeno() was called on a directory")
        endif()

        # Generate file from template
        message(STATUS " from '${template_file}'")
        message(STATUS " to   '${generated_file}'")
        add_custom_command(
            COMMENT "cogeno ${generated_file}"
            OUTPUT ${generated_file}
            MAIN_DEPENDENCY ${template_file}
            DEPENDS
            ${COGENO_DEPENDS}
            COMMAND
            ${COGENO_EXECUTABLE}
            ${COGENO_EXECUTABLE_OPTION}
            ${COGENO_COGENO_DEFINES}
            -D "\"APPLICATION_SOURCE_DIR=${APPLICATION_SOURCE_DIR}\""
            -D "\"APPLICATION_BINARY_DIR=${APPLICATION_BINARY_DIR}\""
            -D "\"PROJECT_NAME=${PROJECT_NAME}\""
            -D "\"PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}\""
            -D "\"PROJECT_BINARY_DIR=${PROJECT_BINARY_DIR}\""
            -D "\"CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}\""
            -D "\"CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}\""
            -D "\"CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}\""
            -D "\"CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}\""
            -D "\"CMAKE_CURRENT_LIST_DIR=${CMAKE_CURRENT_LIST_DIR}\""
            -D "\"CMAKE_FILES_DIRECTORY=${CMAKE_FILES_DIRECTORY}\""
            -D "\"CMAKE_PROJECT_NAME=${CMAKE_PROJECT_NAME}\""
            -D "\"CMAKE_SYSTEM=${CMAKE_SYSTEM}\""
            -D "\"CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}\""
            -D "\"CMAKE_SYSTEM_VERSION=${CMAKE_SYSTEM_VERSION}\""
            -D "\"CMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}\""
            -D "\"CMAKE_C_COMPILER=${CMAKE_C_COMPILER}\""
            -D "\"CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}\""
            -D "\"CMAKE_COMPILER_IS_GNUCC=${CMAKE_COMPILER_IS_GNUCC}\""
            -D "\"CMAKE_COMPILER_IS_GNUCXX=${CMAKE_COMPILER_IS_GNUCXX}\""
            --cmakecache "${CMAKE_BINARY_DIR}/CMakeCache.txt"
            ${COGENO_CONFIG_OPTION}
            ${COGENO_DTS_OPTION}
            ${COGENO_BINDINGS_OPTION}
            ${COGENO_EDTS_OPTION}
            ${COGENO_MODULES_OPTION}
            ${COGENO_TEMPLATES_OPTION}
            --input "${template_file}"
            --output "${generated_file}"
            --log "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/cogeno.log"
            --lock "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/cogeno.lock"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
        target_sources(${target} PRIVATE ${generated_file})
        # Add template directory to include path to allow includes with
        # relative path in generated file to work
        target_include_directories(${target} PRIVATE ${template_dir})
        # Add directory of generated file to include path to allow includes
        # of generated header file with relative path.
        target_include_directories(${target} PRIVATE ${generated_dir})
    endforeach()
endfunction()
