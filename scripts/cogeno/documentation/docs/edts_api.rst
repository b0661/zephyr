..
    Copyright (c) 2018 Bobby Noelte
    SPDX-License-Identifier: Apache-2.0

.. _cogeno_edts_api:

edtsdb API
##########

.. module:: edtsdb
    :synopsis: Extended DTS database

`edtsdb` is a Python module with two primary classes: ``EDTSDb`` and
``EDTSDevice``.

.. contents::
   :depth: 2
   :local:
   :backlinks: top

:mod:`EDTSDb`
=============

.. doxygenclass:: cogeno::modules::edtsdb::database::EDTSDb
    :project: cogeno
    :members:
    :undoc-members:

The ``EDTSDb`` class includes (sub-classes) several mixin classes:

.. doxygenclass:: cogeno::modules::edtsdb::consumer::EDTSConsumerMixin
    :project: cogeno
    :members:
    :undoc-members:

.. doxygenclass:: cogeno::modules::edtsdb::provider::EDTSProviderMixin
    :project: cogeno
    :members:
    :undoc-members:

.. doxygenclass:: cogeno::modules::edtsdb::extractor::EDTSExtractorMixin
    :project: cogeno
    :members:
    :undoc-members:



:mod:`EDTSDevice`
=================

.. doxygenclass:: device::EDTSDevice
    :project: cogeno
    :members:
    :undoc-members:

:mod:`EDTSPinController`
========================

.. doxygenclass:: pincontroller::EDTSPinController
    :project: cogeno
    :members:
    :undoc-members:
