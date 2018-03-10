..
    Copyright (c) 2018 Bobby Noelte
    SPDX-License-Identifier: Apache-2.0

.. _cogeno_invoke_edtsdatabase:

Invoking edtsdatabase
#####################

Synopsis
********

edtsdatabase [OPTIONS]

Description
***********

The Extended Device Tree Specification database collates
device tree (dts) information with information taken from
the device tree bindings.

The EDTS database may be loaded from a json file, stored
to a json file or extracted from the DTS files and the
bindings yaml files.

Options
*******

The following options are understood:

``-h, --help``
    show this help message and exit

``-l FILE, --load FILE``
    Load the input from FILE.

``-s FILE, --save FILE``
    Save the database to FILE.

``-e FILE, --extract FILE``
    Extract the database from dts FILE.

``-b DIR [DIR ...], --bindings DIR [DIR ...]``
    Use bindings from bindings DIR for extraction. We allow multiple.

``-p, --print``
    Print EDTS database content.
