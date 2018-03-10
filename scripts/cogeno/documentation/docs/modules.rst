..
    Copyright (c) 2018 Bobby Noelte
    SPDX-License-Identifier: Apache-2.0

.. _cogeno_modules:

Code generation modules
#######################

Code generation modules provide supporting functions for code generation.

Some modules have to be imported to gain access to the module's functions
and variables. The standard modules are accessible by convenience functions.

 ::

    /* This file uses modules. */
    ...
    /**
     * @code{.cogeno.py}
     * cogeno.import_module('my_special_module')
     * my_special_module.do_everything():
     * @endcode{.cogeno.py}
     */
    /** @code{.cogeno.ins}@endcode */
    ...

.. contents::
   :depth: 2
   :local:
   :backlinks: top

Standard modules
****************

.. toctree::
   :maxdepth: 3
   :caption: Contents:

   edts

Other modules
*************

.. toctree::
   :maxdepth: 3
   :caption: Contents:

   edts




