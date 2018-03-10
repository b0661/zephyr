..
    Copyright (c) 2018 Bobby Noelte
    SPDX-License-Identifier: Apache-2.0

.. _cogeno_build:

Integration into the build process
##################################

Code generation has to be invoked as part of the build process of a project.

.. contents::
   :depth: 2
   :local:
   :backlinks: top

CMake
-----

Projects that use `CMake <https://cmake.org/>`_ to manage building the project
can add the following CMake code to the CMake scripts.

By this a file that contains inline code generation can be added to the project
using the `target_sources_cogeno` command in the respective :file:`CMakeList.txt` file.

.. function:: target_sources_cogeno(file [COGENO_DEFINES defines..] [DEPENDS target.. file..])

.. literalinclude:: ../../examples/cmake/cogeno.cmake
    :language: cmake
    :lines: 4-

Zephyr
------

Cogeno can be integrated into `Zephyr <https://github.com/zephyrproject-rtos/zephyr>`_ by
applying the `codegen pull request <https://github.com/zephyrproject-rtos/zephyr/pull/10885>`_.

Within Zephyr cogeno is referenced as codegen. This was the name the development started with.
It had to be changed because codegen was already used by several open source project.

In Zephyr the processing of source files is controlled by the CMake extension functions:
``zephyr_sources_codegen(..)`` or ``zephyr_library_sources_codegen(..)``. The generated
source files are added to the Zephyr sources. During build the source files are
processed by cogeno and the generated source files are written to the CMake
binary directory. Zephyr uses `CMake <https://cmake.org/>`_ as the tool to manage building
the project. A file that contains inline code generation has to be added to the project
by one of the following commands in a :file:`CMakeList.txt` file:

.. function:: zephyr_sources_cogdeen(file [CODEGEN_DEFINES defines..] [DEPENDS target.. file..])

.. function:: zephyr_sources_codegen_ifdef(ifguard file [CODEGEN_DEFINES defines..] [DEPENDS target.. file..])

.. function:: zephyr_library_sources_codegen(file [CODEGEN_DEFINES defines..] [DEPENDS target.. file..])

.. function:: zephyr_library_sources_codegen_ifdef(ifguard file [CODEGEN_DEFINES defines..] [DEPENDS target.. file..])

The arguments given by the ``COGDEGEN_DEFINES`` keyword have to be of the form
``define_name=define_value``. The arguments become globals in the python
snippets and can be accessed by ``define_name``.

Dependencies given by the ``DEPENDS`` key word are added to the dependencies
of the generated file.
