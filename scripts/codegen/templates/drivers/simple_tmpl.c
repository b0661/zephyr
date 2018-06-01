/*
 * Copyright (c) 2018 Bobby Noelte
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * Code generation template for simple drivers
 */

/**
 * @brief simple drivers template.
 * @defgroup simple_template simple drivers template.
 * @ingroup device_driver_support
 *
 * Code generation template for simple drivers.
 *
 * Usage example UART:
 * device_type = 'SERIAL'
 * device_configs = ['CONFIG_UART_STM32_PORT_{}'.format(x)
 *                                               for x in range(1, 11)] +
 *                  ['CONFIG_UART_STM32_PORT_{}'.format(x)
 *                                               for x in range(1, 11)] +
 *                  ['CONFIG_UART_STM32_LPUART_{}'.format(x)
 *                                                 for x in range(1, 2)]
 * driver_names = ['"UART_{}"'.format(x) for x in range(1, 11)] +
 *                ['"USART_{}"'.format(x) for x in range(1, 11)] +
 *                ['"LPUART_{}"'.format(x) for x in range(1, 2)]
 * device_inits = ['uart_stm32_init' for x in range(1, 11)] +
 *                ['uart_stm32_init' for x in range(1, 11)] +
 *                ['uart_stm32_init' for x in range(1, 2)]
 * device_levels = 'PRE_KERNEL_1'
 * device_prios = 'CONFIG_KERNEL_INIT_PRIORITY_DEVICE'
 * device_api = 'uart_stm32_driver_api'
 * device_data = ('struct uart_stm32_data', )
 * device_config_info = ('struct uart_stm32_config',
 * """
 *      .uconf.base = (u8_t *)$BASE_ADDRESS,
 *      .uconf.irq_config_func = uart_stm32_irq_config_func_${DEVICE_NAME},
 *      .pclken.bus = $CLOCK_0_BUS,
 *      .pclken.bits = $CLOCK_0_BITS,
 *      .baud_rate = CONFIG_UART_STM32_${DRIVER_NAME}_BAUD_RATE,
 * """)
 * device_config_irq =
 * """
 * static void uart_stm32_irq_config_func_${DEVICE_NAME}(struct device *dev)
 * {
 *      IRQ_CONNECT(${DRIVER_NAME}_IRQ
 *              CONFIG_UART_STM32_${DRIVER_NAME}_IRQ_PRI
 *              uart_stm32_isr, DEVICE_GET(uart_stm32_${DRIVER_NAME})
 *              0);
 *      irq_enable(${DRIVER_NAME}_IRQ);
 * }
 * """
 * codegen.out_include('templates/drivers/simple_tmpl.c')
 *
 * The template expects the following globals to be set:
 * - device_type
 *   Type of device (e.g. 'UART', 'GPIO', ...)
 * - device_configs
 *   A list of configuration variables for device instantiation.
 *   (e.g. ['CONFIG_SPI_0', 'CONFIG_SPI_1'])
 * - driver_names
 *   A list of driver names for device instantiation. The list shall be ordered
 *   as the list of device configs. Watch for the '"' of the driver names.
 *   (e.g. ['"SPI_0"', '"SPI_1"'])
 * - device_inits
 *   A list of device initialisation functions or a one single function. The
 *   list shall be ordered as the list of device configs.
 *   (e.g. 'spi_stm32_init')
 * - device_levels
 *   A list of driver initialisation levels or one single level definition. The
 *   list shall be ordered as the list of device configs.
 *   (e.g. 'PRE_KERNEL_1')
 * - device_prios
 *   A list of driver initialisation priorities or one single priority
 *   definition. The list shall be ordered as the list of device configs.
 *   (e.g. 32)
 * - device_api
 *   Identifier of the device api.
 *   (e.g. 'spi_stm32_driver_api')
 * - device_data
 *   device data type definition (e.g. 'struct pinctrl_stm32_data') and
 *   an optional code template for data initialisation.
 * - device_config_info
 *   Device config type definition (e.g. 'struct pinctrl_stm32_data') and
 *   a code template for configuration initialisation.
 * - device_config_irq
 *   A code template for interrupt configuration initialisation.
 *
 * @{
 */

/**
 * @code{.codegen}
 *
 * # template shall only be included once
 * codegen.guard_include()
 *
 * # Basic device tree data access
 * codegen.out_include('templates/dts/basic_tmpl.c')
 *
 * from string import Template
 * _template_dts_aliases = {
 *     'BASE_ADDRESS' : 'reg0',
 *     'LABEL' : 'label',
 * }
 *
 * def _template_substitute(template, device_name, mapping={}):
 *     mapping['DEVICE_NAME'] = device_name
 *     mapping['DRIVER_NAME'] = mapping.get('DRIVER_NAME',
 *         codegen.device_tree_property(
 *             '{}_LABEL'.format(device_name)).strip('"'))
 *     for property_name in codegen.device_tree_properties():
 *         property_value = codegen.device_tree_property(property_name)
 *         if property_name.startswith(device_name):
 *             identifier = property_name.replace(device_name, '').lstrip('_')
 *             mapping[identifier] = property_value
 *             if identifier in _template_dts_aliases:
 *                 mapping[_template_dts_aliases[identifier]] = property_value
 *         mapping[property_name] = property_value
 *     return Template(template).safe_substitute(mapping)
 *
 * for i, _dev_config in enumerate(device_configs):
 *     _dev_configured = codegen.config_property(_dev_config, None)
 *     if _dev_configured is None:
 *         continue
 *     _drv_name = driver_names[i]
 *     if isinstance(device_inits, str):
 *         _dev_init = device_inits
 *     else:
 *         try:
 *             _dev_init = device_inits[i]
 *         except:
 *             _dev_init = device_inits
 *     if isinstance(device_levels, str):
 *         _dev_level = device_levels
 *     else:
 *         try:
 *             _dev_level = device_levels[i]
 *         except:
 *             _dev_level = device_levels
 *     if isinstance(device_prios, str):
 *         _dev_prio = device_prios
 *         try:
 *             _dev_prio = device_prios[i]
 *         except:
 *             _dev_prio = device_prios
 *     x = device_tree_controller_device_index(device_type, _drv_name)
 *     if x is None:
 *         # this should not happen
 *         codegen.prerr("Did not find {}.".format(_drv_name))
 *         continue
 *     _dev_name = device_tree_controller_device_name(device_type, x)
 *     _config_info = device_tree_controller_data_name(
 *                                 device_type, x, 'config')
 *     _data = device_tree_controller_data_name(device_type, x, 'data')
 *     #
 *     # config irq
 *     codegen.outl(_template_substitute(device_config_irq, _dev_name))
 *     #
 *     # config info
 *     codegen.outl('static const {} {} = {{'.format(
 *         device_config_info[0], _config_info))
 *     codegen.outl(_template_substitute(device_config_info[1], _dev_name))
 *     codegen.outl('};')
 *     #
 *     # data
 *     codegen.outl('static {} {} = {{'.format(
 *         device_data[0], _data))
 *     try:
 *         codegen.outl(_template_substitute(device_data[1], _dev_name))
 *     except:
 *         pass
 *     codegen.outl('};')
 *     #
 *     # device init
 *     codegen.outl('DEVICE_AND_API_INIT( \\')
 *     codegen.outl('\t{}, \\'.format(_dev_name))
 *     codegen.outl('\t{}, \\'.format(_drv_name))
 *     codegen.outl('\t{}, \\'.format(_dev_init))
 *     codegen.outl('\t&{}, \\'.format(_data))
 *     codegen.outl('\t&{}, \\'.format(_config_info))
 *     codegen.outl('\t{}, \\'.format(_dev_level))
 *     codegen.outl('\t{}, \\'.format(_dev_prio))
 *     codegen.outl('\t&{});'.format(device_api))
 *
 * @endcode{.codegen}
 */
/** @code{.codeins}@endcode */

/** @} simple_template */
