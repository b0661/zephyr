..
    Copyright (c) 2018 Bobby Noelte
    SPDX-License-Identifier: Apache-2.0

.. _codegen:

Inline code generation
######################

For some repetitive or parameterized coding tasks, it's convenient to
use a code generating tool to build code fragments, instead of writing
(or editing) that source code by hand.

Cogeno, the inline code generation tool, processes Python or Jinja2 "snippets"
inlined in your source files. It can also access CMake build
parameters and device tree information to generate source code automatically
tailored and tuned to a specific project configuration.

Cogeno can be used, for example, to generate source code that creates
and fills data structures, adapts programming logic, creates
configuration-specific code fragments, and more.

Cogeno is developed in its own `repository on GitLab <https://gitlab.com/b0661/cogeno>`_.
A copy is maintained in Zephyr's :file:`scripts/cogeno` directory.

Documentation
*************

Cogeno's documentation is available:

  - online at `Read the Docs <https://cogeno.readthedocs.io/en/latest/index.html>`_.
  - in the `repository on GitLab <https://gitlab.com/b0661/cogeno>`_.

Zephyr specific aspects can be found in:

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   build
   modules
   templates
