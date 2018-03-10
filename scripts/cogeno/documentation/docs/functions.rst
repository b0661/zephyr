..
    Copyright (c) 2004-2015 Ned Batchelder
    SPDX-License-Identifier: MIT
    Copyright (c) 2018 Bobby Noelte
    SPDX-License-Identifier: Apache-2.0

.. _cogeno_functions:

Code generation functions
#########################

A module called ``cogeno`` provides the core functions for inline
code generation. It encapsulates all the functions to retrieve information
(options, device tree properties, CMake variables, config properties) and
to output the generated code.

.. contents::
   :depth: 2
   :local:
   :backlinks: top

The ``cogeno`` module is automatically imported by all code snippets. No
explicit import is necessary.

Output
------

.. function:: cogeno.out(sOut=’’ [, dedent=False][, trimblanklines=False])

    Writes text to the output.

    :param sOut: The string to write to the output.
    :param dedent: If dedent is True, then common initial white space is
                   removed from the lines in sOut before adding them to the
                   output.
    :param trimblanklines: If trimblanklines is True,
                           then an initial and trailing blank line are removed
                           from sOut before adding them to the output.

    ``dedent`` and ``trimblanklines`` make it easier to use
    multi-line strings, and they are only are useful for multi-line strings:

    ::

        cogeno.out("""
            These are lines I
            want to write into my source file.
        """, dedent=True, trimblanklines=True)

.. function:: cogeno.outl

    Same as cogeno.out, but adds a trailing newline.

.. attribute:: cogeno.inFile

    An attribute, the path of the input file.

.. attribute:: cogeno.outFile

    An attribute, the path of the output file.

.. attribute:: cogeno.firstLineNum

    An attribute, the line number of the first line of Python code in the
    generator. This can be used to distinguish between two generators in the
    same input file, if needed.

.. attribute:: cogeno.previous

    An attribute, the text output of the previous run of this generator. This
    can be used for whatever purpose you like, including outputting again with
    cogeno.out()

The cogen module also provides a set of convenience functions:


Code generation module import
-----------------------------

.. function:: cogeno.module_import(module_name)

    Import a module from the cogen/modules package.

    After import the module's functions and variables can be accessed by
    module_name.func() and module_name.var.

    :param module_name: Module to import. Specified without any path.

    See :ref:`cogeno_modules` for the available modules.

Template file inclusion
-----------------------

.. function:: cogeno.out_include(include_file)

    Write the text from include_file to the output. The :file:`include_file`
    is processed by cogeno. Inline code generation in ``include_file``
    can access the globals defined in the ``including source file`` before
    inclusion. The ``including source file`` can access the globals defined in
    the ``include_file`` (after inclusion).

    :param include_file: path of include file, either absolute path or relative
                         to current directory or relative to templates directory
                         (e.g. 'templates/drivers/simple_tmpl.c')

    See :ref:`cogeno_templates` for the templates in the cogeno templates
    folders.

.. function:: cogeno.guard_include()

   Prevent the current file to be included by ``cogeno.out_include()``
   when called the next time.

Configuration property access
-----------------------------

.. doxygenfunction:: config_property
    :project: cogeno

.. function:: cogeno.config_property(property_name [, default="<unset>"])

    Get the value of a configuration property from :file:`.config`. If
    ``property_name`` is not given in :file:`.config` the default value is
    returned.

    See :ref:`cogeno_invoke_cogeno` and :ref:`cogeno_build` for how to provide config
    variables to cogeno.

CMake variable access
---------------------

.. function:: cogeno.cmake_variable(variable_name [, default="<unset>"])

    Get the value of a CMake variable. If variable_name is not provided to
    cogeno by CMake the default value is returned.

    See :ref:`cogeno_invoke_cogeno` and :ref:`cogeno_build` for how to provide CMake
    variables to cogeno.

    A typical set of CMake variables that are not available in the
    :file:`CMakeCache.txt` file and have to be provided as defines
    to cogeno if needed:

    - "PROJECT_NAME"
    - "PROJECT_SOURCE_DIR"
    - "PROJECT_BINARY_DIR"
    - "CMAKE_SOURCE_DIR"
    - "CMAKE_BINARY_DIR"
    - "CMAKE_CURRENT_SOURCE_DIR"
    - "CMAKE_CURRENT_BINARY_DIR"
    - "CMAKE_CURRENT_LIST_DIR"
    - "CMAKE_FILES_DIRECTORY"
    - "CMAKE_PROJECT_NAME"
    - "CMAKE_SYSTEM"
    - "CMAKE_SYSTEM_NAME"
    - "CMAKE_SYSTEM_VERSION"
    - "CMAKE_SYSTEM_PROCESSOR"
    - "CMAKE_C_COMPILER"
    - "CMAKE_CXX_COMPILER"
    - "CMAKE_COMPILER_IS_GNUCC"
    - "CMAKE_COMPILER_IS_GNUCXX"

.. function:: cogeno.cmake_cache_variable(variable_name [, default="<unset>"])

    Get the value of a CMake variable from CMakeCache.txt. If variable_name
    is not given in CMakeCache.txt the default value is returned.

Extended device tree database access
------------------------------------

.. function:: cogeno.edts()

    Get the extended device tree database.

    :return: extended device tree database

    See :ref:`cogeno_invoke_cogeno` and :ref:`cogeno_build` for how to provide all
    files to enable cogeno to build the extended device tree database.

Guarding chunks of source code
------------------------------

.. function:: cogeno.outl_guard_config(property_name)

    Write a guard (#if [guard]) C preprocessor directive to output.

    If there is a configuration property of the given name the property value
    is used as guard value, otherwise it is set to 0.

    :param property_name: Name of the configuration property.

.. function:: cogeno.outl_unguard_config(property_name)

    Write an unguard (#endif) C preprocessor directive to output.

    This is the closing command for cogeno.outl_guard_config().

    :param property_name: Name of the configuration property.

Error handling
--------------

.. function:: cogeno.error(msg='Error raised by cogeno.' [, frame_index=0] [, snippet_lineno=0])

    Raise a cogeno.Error exception.

    Instead of raising standard python errors, cogen generators can use
    this function. Extra information is added that maps the python snippet
    line seen by the Python interpreter to the line of the file that inlines
    the python snippet.

    :param msg: Exception message.
    :param frame_index: Call frame index. The call frame offset of the function
                        calling cogeno.error(). Zero if directly called in a
                        snippet. Add one for every level of function call.
    :param snippet_lineno: Line number within snippet.

Logging
-------

.. function:: cogeno.log(message [, message_type=None] [, end="\n"] [, logonly=True])

.. function:: cogeno.msg(msg)

    Prints msg to stdout with a “Message: ” prefix.

.. function:: cogeno.prout(s [, end="\n"])

.. function:: cogeno.prerr(s [, end="\n"])
