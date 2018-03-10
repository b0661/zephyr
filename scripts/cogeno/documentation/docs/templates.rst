..
    Copyright (c) 2018 Bobby Noelte
    SPDX-License-Identifier: Apache-2.0

.. _cogeno_templates:

Code Generation Templates
#########################

Code generation templates provide sophisticated code generation functions.

Templates are simply text files. They may be hierarchical organized.
There is always one top level template. All the other templates have
to be included to gain access to the template's functions and variables.

A template file usually contains normal text and templating commands
intermixed. A bound sequence of templating commands is called a script
snippet. As a special case a template file may be a scripte snippet
as a whole.

Cogeno supports two flavours of script snippets: Python and Jinja2.
A script snippet has to be written in one of the two scripting
languages. Within a template file snippets of different language can
coexist.

.. contents::
   :depth: 2
   :local:
   :backlinks: top


Template Snippets
*****************


 ::

    /* This file uses templates. */
    ...
    /**
     * @code{.cogen}
     * template_in_var = 1
     * cogeno.out_include('templates/template_tmpl.c')
     * if template_out_var not None:
     *     cogeno.outl("int x = %s;" % template_out_var)
     * @endcode{.cogen}
     */
    /** @code{.codeins}@endcode */
    ...
