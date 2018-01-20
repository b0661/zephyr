.. _device_drivers_gpio:

GPIO (general pupose input output) device driver
################################################

The **general pupose input output** device driver deals with:

- input and output on **ports** and **pins**,
- interrupt generation on **pins**,
- configuration of **ports** and **pins**

General pupose input output concepts
************************************

A **GPIO device driver**
    provides access to a GPIO *port*.

A GPIO **port**
    is a group of *pins* arranged in a group and controllable as a group.

A **pin**
    is the physical input/ output line of a chip. The pin
    numberspace is local [#pin_number_space]_ to a *port*. This
    pin space may be sparse - i.e. there may be gaps in the space with numbers
    where no pin exists.

GPIO device driver
******************

The GPIO device driver provides applications and other drivers to:

- read from a GPIO port or port pin,
- write to GPIO port or port pin,
- get signalled on GPIO port or port pin changes or interrupts.

The GPIO device driver API is provided by :file:`include/gpio.h`.

Control properties
==================

Pins
----

The pins are defined in the generic GPIO include file
:file:`include/gpio_common.h` (eg. :c:macro:`GPIO_PORT_PIN0`).

The maximum number of pins managed by a GPIO device driver is 32.

Pin Configuration
=================

Pin configuration by the GPIO pin configuration interface is deprecated.
Please use the PINCTRL pin configuration interface instead.

A generic set of GPIO port pin configuration properties is defined in
:file:`include/gpio.h`.
The GPIO pin configuration properties can be mapped to the generic pin
configuration properties of the PINCTRL device that controls the GPIO
port pins. A GPIO driver implementation using the PINCTRL device as a backend
may just do this.

**GPIO_DIR_IN**
    GPIO pin to be input.

    PINCTRL pin configuration property:

    - :c:macro:`PINCTRL_CONFIG_INPUT_ENABLE`,
    - :c:macro:`PINCTRL_CONFIG_OUTPUT_DISABLE`

**GPIO_DIR_OUT**
    GPIO pin to be output.

    PINCTRL pin configuration property:

    - :c:macro:`PINCTRL_CONFIG_INPUT_ENABLE`,
    - :c:macro:`PINCTRL_CONFIG_OUTPUT_ENABLE`

**GPIO_POL_NORMAL**
    GPIO pin polarity is normal.

    PINCTRL pin configuration property:

    - :c:macro:`PINCTRL_CONFIG_OUTPUT_HIGH`

**GPIO_POL_INV**
    GPIO pin polarity is inverted.

    PINCTRL pin configuration property:

    - :c:macro:`PINCTRL_CONFIG_OUTPUT_LOW`

**GPIO_PUD_NORMAL**
    GPIO pin to have no pull-up or pull-down.

    PINCTRL pin configuration property:

    - :c:macro:`PINCTRL_CONFIG_BIAS_DISABLE`

**GPIO_PUD_PULL_UP**
    Enable GPIO pin pull-up.

    PINCTRL pin configuration property:

    - :c:macro:`PINCTRL_CONFIG_BIAS_PULL_UP`

**GPIO_PUD_PULL_DOWN**
    Enable GPIO pin pull-down.

    PINCTRL pin configuration property:

    - :c:macro:`PINCTRL_CONFIG_BIAS_PULL_DOWN`

**GPIO_DS_DFLT_LOW**
    Default drive strength standard when GPIO pin output is low.

    PINCTRL pin configuration property:

    - :c:macro:`PINCTRL_CONFIG_DRIVE_STRENGTH_DEFAULT`

**GPIO_DS_DFLT_HIGH**
    Default drive strength when GPIO pin output is high.

    PINCTRL pin configuration property:

    - :c:macro:`PINCTRL_CONFIG_DRIVE_STRENGTH_DEFAULT`

**GPIO_DS_ALT_LOW**
    Alternative drive strength when GPIO pin output is low.

    PINCTRL pin configuration property:

    - :c:macro:`PINCTRL_CONFIG_DRIVE_STRENGTH_7`

**GPIO_DS_ALT_HIGH**
    Alternative drive strength when GPIO pin output is high.

    PINCTRL pin configuration property:

    - :c:macro:`PINCTRL_CONFIG_DRIVE_STRENGTH_7`

**GPIO_DS_DISCONNECT_LOW**
    Disconnect pin when GPIO pin output is low.

    PINCTRL pin configuration property:

    - :c:macro:`PINCTRL_CONFIG_DRIVE_OPEN_SOURCE`

**GPIO_DS_DISCONNECT_HIGH**
    Disconnect pin when GPIO pin output is high.

    PINCTRL pin configuration property:

    - :c:macro:`PINCTRL_CONFIG_DRIVE_OPEN_DRAIN`

Signalling Configuration
========================

Pin signalling can be configured by the general GPIO configuration interface.

:c:func:`gpio_pin_configure`
    Set the configuration of a pin.

A generic set of GPIO port pin signalling properties is defined in
:file:`include/gpio.h`. A GPIO driver may only support a subset of the
signalling properties.

**GPIO_INT**
    GPIO pin to trigger interrupt.

**GPIO_INT_ACTIVE_LOW**
    GPIO pin trigger on level low or falling edge.

**GPIO_INT_ACTIVE_HIGH**
    GPIO pin trigger on level high or rising edge.

**GPIO_INT_CLOCK_SYNC**
    GPIO pin trigger to be synchronized to clock pulses.

**GPIO_INT_DEBOUNCE**
    Enable GPIO pin trigger debounce.

**GPIO_INT_LEVEL**
    Do Level trigger.

**GPIO_INT_EDGE**
    Do Edge trigger.

**GPIO_INT_DOUBLE_EDGE**
    Interrupt triggers on both rising and falling edge.


GPIO device tree support
************************

Nodes
=====

GPIO Controller Node
--------------------

The GPIO device is defined by a GPIO controller node.

Required properties are:

    - gpio-controller: Indicates this device is a GPIO controller
    - compatible:
    - label: Should be a name string


Interaction with the PINCTRL device driver
==========================================

To enable a pin controller to report the pins that are associated to a GPIO device
the gpio-ranges property shall be set in the device tree. Also the label property
shall be set and used as device name.

The gpio-ranges property is of the form:

.. code-block:: text

    gpio-ranges = <&[pin controller] [pin number in GPIO pin number space]
                                     [pin number in pin controller number space]
                                     [number of pins]>,
                                     /* ... */
                                     ;

A GPIO port definition in the device tree may look like:

.. code-block:: DTS

    pinctrl: pin-controller@48000000 {
        pin-controller;
        /* ... */
    };

    gpioa: gpio@40020000 {
        gpio-controller;
        compatible = "st,stm32-gpio-pinctrl";
        #gpio-cells = <2>;
        reg = <0x40020000 0x400>;
        label = "STM32_GPIOA";
        gpio-ranges = <&pinctrl GPIO_PORT_PIN0 PINCTRL_STM32_PINA0 16>;
    };


GPIO device driver implementation
*********************************

Driver configuration information comes from three sources:
    - autoconf.h (Kconfig)
    - generated_dts_board.h (device tree)
    - soc.h

Driver configuration by autoconf
================================

See :ref:`configuration` for the the full set of Kconfig symbols that are available.
Specific to the GPIO driver are:

:c:macro:`CONFIG_GPIO`
    Enable GPIO driver.

Driver configuration by device tree
===================================

The general device information for GPIO controllers is:

:c:macro:`SOC_GPIO_CONTROLLER_COUNT`
    Number of GPIO controllers that are activated. All nodes that contain a
    gpio-controller directive are counted.

:c:macro:`SOC_GPIO_CONTROLLER_0` ...
    The label prefix of GPIO controller 0 ... (e.g. 'ST_STM32_GPIO_PINCTRL_48000000').
    The define is generated if a gpio-controller directive is given in the node.

For a GPIO controller node the following information is extracted from the device tree
and provided in :file:`generated_dts_board.h`.

**TBD**

Driver configuration by soc.h
=============================

:file:`soc.h` may contain driver relevant information like e.g. HAL header files.

Drivers
=======

.. toctree::
   :maxdepth: 1

   gpio_stm32.rst

Design Rationales
*****************

.. [#pin_number_space] GPIO uses a **local pin number space** instead of
   a global space to allow for object encapsulation.


References
**********
