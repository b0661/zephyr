..
    Copyright (c) 2004-2015 Ned Batchelder
    SPDX-License-Identifier: MIT
    Copyright (c) 2018 Bobby Noelte
    SPDX-License-Identifier: Apache-2.0

.. _cogeno_intro:

Introduction
############

About cogeno
------------

Script snippets that are inlined in a source file are used as code generators.
The tool to scan the source file for the script snippets and process them is
cogeno. Cogeno and part of this documentation is based on
`Cog <https://nedbatchelder.com/code/cog/index.html>`_ from Ned Batchelder.

The inlined script snippets can contain any `Python 3 <https://www.python.org>`_
or `Jinja2 <http://jinja.pocoo.org/>`_ code, they are regular scripts.

All Python snippets in a source file and all Python snippets of
included template files are treated as a python script with a common set of
global Python variables. Global data created in one snippet can be used in
another snippet that is processed later on. This feature could be used, for
example, to customize included template files.

Jinja2 snippets provide a - compared to Python - simplified script language.

An inlined script snippet can always access the cogeno module. The cogeno
module encapsulates and provides all the functions to retrieve information
(options, device tree properties, CMake variables, config properties) and to
output the generated code.

Cogeno transforms files in a very simple way: it finds snippets of script code
embedded in them, executes the script code, and places its output combined with
the original file into the generated file. The original file can contain
whatever text you like around the script code. It will usually be source code.

For example, if you run this file through cogeno:

::

    /* This is my C file. */
    ...
    /**
     * @code{.cogeno.py}
     * fnames = ['DoSomething', 'DoAnotherThing', 'DoLastThing']
     * for fn in fnames:
     *     cogeno.outl("void %s();" % fn)
     * @endcode{.cogeno.py}
     */
    /** @code{.cogeno.ins}@endcode */
    ...

it will come out like this:

::

    /* This is my C file. */
    ...
    /**
     * @code{.cogeno.py}
     * fnames = ['DoSomething', 'DoAnotherThing', 'DoLastThing']
     * for fn in fnames:
     *     cogeno.outl("void %s();" % fn)
     * @endcode{.cogeno.py}
     */
    void DoSomething();
    void DoAnotherThing();
    void DoLastThing();
    /** @code{.cogeno.ins}@endcode */
    ...

Lines with ``@code{.cogeno.py}`` or ``@code{.cogeno.ins}@endcode`` are marker lines.
The lines between ``@code{.cogeno.py}`` and ``@endcode{.cogeno.py}`` are the
generator Python code. The lines between ``@endcode{.cogeno.py}`` and
``@code{.cogeno.ins}@endcode`` are the output from the generator.

When cogeno runs, it discards the last generated Python output, executes the
generator Python code, and writes its generated output into the file. All text
lines outside of the special markers are passed through unchanged.

The cogeno marker lines can contain any text in addition to the marker tokens.
This makes it possible to hide the generator Python code from the source file.

In the sample above, the entire chunk of Python code is a C comment, so the
Python code can be left in place while the file is treated as C code.

About edtsdatabase
------------------

Cogeno includes as a module the Extended Device Tree Specification (EDTS)
database. The database provides access to the device tree specification data of
a project.

The EDTS database may be loaded from a json file, stored to a json file or
extracted from the DTS files and the bindings yaml files of the project.

The EDTS database is automatically available to cogeno scripts. It can also be
used as a standalone tool.

About the documentation
-----------------------

This documentation is continuously written. It is edited via text files in the
reStructuredText markup language and then compiled into a static website/
offline document using the open source Sphinx and
`Read the Docs <https://cogeno.readthedocs.io/en/latest/index.html>`_ tools.

You can contribute to cogeno's dcoumentation by opening
`GitLab issues <https://gitlab.com/b0661/cogeno/issues>`_
or sending patches via merge requests on its
`GitLab repository <https://gitlab.com/b0661/cogeno>`_.
