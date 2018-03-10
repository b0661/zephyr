..
    Copyright (c) 2018 Bobby Noelte
    SPDX-License-Identifier: Apache-2.0

.. _cogeno_edts_bindings:

EDTS bindings
#############

The EDTS database module uses bindings (a kind of data schema) to know what data
to extract and to know the kind of data. A set of generic bindings controls
the extraction process. The generic bindings are part of the EDTS database module.

.. contents::
   :depth: 2
   :local:
   :backlinks: top

Fixed partitions
================

::

    inherits:
        !fixed-partitions.yaml

.. literalinclude:: ../../cogeno/modules/edtsdb/bindings/fixed-partitions.yaml
    :language: yaml
    :lines: 6-

Flash
=====

::

    inherits:
        !include flash.yaml

.. literalinclude:: ../../cogeno/modules/edtsdb/bindings/flash.yaml
    :language: yaml
    :lines: 6-

Flash controller
================

::

    inherits:
        !include flash-controller.yaml

.. literalinclude:: ../../cogeno/modules/edtsdb/bindings/flash-controller.yaml
    :language: yaml
    :lines: 6-

Partitions
==========

::

    inherits:
        !partitions.yaml

.. literalinclude:: ../../cogeno/modules/edtsdb/bindings/partitions.yaml
    :language: yaml
    :lines: 6-

SoC non-volatile flash
======================

::

    inherits:
        !include soc-nv-flash.yaml

.. literalinclude:: ../../cogeno/modules/edtsdb/bindings/soc-nv-flash.yaml
    :language: yaml
    :lines: 6-


