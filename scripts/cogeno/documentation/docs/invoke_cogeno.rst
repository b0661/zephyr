..
    Copyright (c) 2018 Bobby Noelte
    SPDX-License-Identifier: Apache-2.0

.. _cogeno_invoke_cogeno:

Invoking cogeno
###############

Synopsis
********

cogeno [OPTIONS]

Description
***********

Cogeno transforms files in a very simple way: it finds chunks of script code
embedded in them, executes the script code, and places its output combined with
the original file content into the generated file. It supports Python and Jinja2
scripts.

Options
*******

The following options are understood:

``-h, --help``
    show this help message and exit

``-x, --delete-code``
    Delete the generator code from the output file.

``-w, --warn-empty``
    Warn if a file has no generator code in it.

``-n ENCODING, --encoding ENCODING``
    Use ENCODING when reading and writing files.

``-U, --unix-newlines``
    Write the output with Unix newlines (only LF line-endings).

``-D DEFINE, --define DEFINE``
    Define a global string available to your generator code.

``-m DIR [DIR ...], --modules DIR [DIR ...]``
    Use modules from modules DIR. We allow multiple

``-t DIR [DIR ...], --templates DIR [DIR ...]``
    Use templates from templates DIR. We allow multiple

``-c FILE, --config FILE``
    Use configuration variables from configuration FILE.

``-k FILE, --cmakecache FILE``
    Use CMake variables from CMake cache FILE.

``-d FILE, --dts FILE``
    Load the device tree specification from FILE.

``-b DIR [DIR ...], --bindings DIR [DIR ...]``
    Use bindings from bindings DIR for device tree extraction. We allow multiple.

``-e FILE, --edts FILE``
    Write or read EDTS database to/ from FILE.

``-i FILE, --input FILE``
    Get the input from FILE.

``-o FILE, --output FILE``
    Write the output to FILE.

``-l FILE, --log FILE``
    Log to FILE.

``-k FILE, --lock FILE``
    Use lock FILE for concurrent runs of cogeno.

