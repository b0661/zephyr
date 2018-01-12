/*
 * Copyright (c) 2018 Bobby Noelte
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * Library for PINCTRL drivers
 */

#ifndef __PINCTRL_LIB_H
#define __PINCTRL_LIB_H

/**
 * @brief PINCTRL Library.
 * @defgroup pinctrl_library PINCTRL Library.
 * @ingroup device_driver_support
 *
 * Library for PINCTRL drivers.
 *
 * The library uses the CHAOS preprocessor macro library to
 * generate and fill data structures for pin control.
 *
 * @{
 */

/**
 * @def CONFIG_PINCTRL_LIB_SELECT_PINMUX
 * @brief Select pin configuration by pinmux property [0,1].
 * @def CONFIG_PINCTRL_LIB_SELECT_GROUP
 * @brief Select pin configuration by group property [0,1].
 * @def CONFIG_PINCTRL_LIB_SELECT_PINS
 * @brief Select pin configuration by pins property [0,1].
 * @def CONFIG_PINCTRL_LIB_ENABLE_MUX_REQUEST
 * @brief Enable/ disable mux request/ free  [0,1].
 * @def CONFIG_PINCTRL_LIB_DEVICE_NAME_PREFIX
 * @brief C function prefix for pinctrl devices (e.g. pinctrl_stm32)
 * @def CONFIG_PINCTRL_LIB_CONFIG_GET
 * @brief C function name of device config_get function.
 * @def CONFIG_PINCTRL_LIB_CONFIG_SET
 * @brief C function name of device mux_free function.
 * @def CONFIG_PINCTRL_LIB_MUX_GET
 * @brief C function name of device mux_get function.
 * @def CONFIG_PINCTRL_LIB_MUX_SET
 * @brief C function name of device mux_set function.
 * @def CONFIG_PINCTRL_LIB_PINCONF_PINMUX_PIN
 * @brief Name of C macro that extracts pin number from pinmux property.
 * @def CONFIG_PINCTRL_LIB_PINCONF_PINMUX_MUX
 * @brief Name of C macro that extracts mux value from pinmux property.
 * @def CONFIG_PINCTRL_LIB_CONTROLLER_COUNT
 * @brief Number of pin controllers to be generated.
 * @def CONFIG_PINCTRL_LIB_CONTROLLER_0_PREFIX
 * @brief Prefix of device tree defines of controller 0.
 * @def CONFIG_PINCTRL_LIB_CONTROLLER_0_DATA
 * @brief Address of data of controller 0 (e.g. pinctrl_stm32_data[0]).
 * @def CONFIG_PINCTRL_LIB_CONTROLLER_0_PIN_COUNT
 * @brief Number of pins controlled by controller 0.
 * @def CONFIG_PINCTRL_LIB_CONTROLLER_0_DEVICE_INIT
 * @brief C function name of device init function for controller 0.
 * @def CONFIG_PINCTRL_LIB_CONTROLLER_1_PREFIX
 * @brief Prefix of device tree defines of controller 1.
 * @def CONFIG_PINCTRL_LIB_CONTROLLER_1_DATA
 * @brief Address of data of controller 1 (e.g. pinctrl_stm32_data[1]).
 * @def CONFIG_PINCTRL_LIB_CONTROLLER_1_PIN_COUNT
 * @brief Number of pins controlled by controller 1.
 * @def CONFIG_PINCTRL_LIB_CONTROLLER_1_DEVICE_INIT
 * @brief C function name of device init function for controller 1.
 * @def CONFIG_PINCTRL_LIB_CONTROLLER_2_PREFIX
 * @brief Prefix of device tree defines of controller 2.
 * @def CONFIG_PINCTRL_LIB_CONTROLLER_2_DATA
 * @brief Address of data of controller 2 (e.g. pinctrl_stm32_data[2]).
 * @def CONFIG_PINCTRL_LIB_CONTROLLER_2_PIN_COUNT
 * @brief Number of pins controlled by controller 2.
 * @def CONFIG_PINCTRL_LIB_CONTROLLER_2_DEVICE_INIT
 * @brief C function name of device init function for controller 2.
 * @def CONFIG_PINCTRL_LIB_CONTROLLER_3_PREFIX
 * @brief Prefix of device tree defines of controller 3.
 * @def CONFIG_PINCTRL_LIB_CONTROLLER_3_DATA
 * @brief Address of data of controller 3 (e.g. pinctrl_stm32_data[3]).
 * @def CONFIG_PINCTRL_LIB_CONTROLLER_3_PIN_COUNT
 * @brief Number of pins controlled by controller 3.
 * @def CONFIG_PINCTRL_LIB_CONTROLLER_3_DEVICE_INIT
 * @brief C function name of device init function for controller 3.
 */
#if CONFIG_PINCTRL_LIB_SELECT_GROUP
#error "PINCTRL Lib: group configuration property not (yet) supported!"
#endif
#if CONFIG_PINCTRL_LIB_SELECT_PINS
#error "PINCTRL Lib: pins configuration property not (yet) supported!"
#endif
#if !CONFIG_PINCTRL_LIB_SELECT_PINMUX
#error "PINCTRL Lib: pinmux configuration property required!"
#endif
#if !defined(CONFIG_PINCTRL_LIB_ENABLE_MUX_REQUEST)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_ENABLE_MUX_REQUEST required!"
#endif
#if !defined(CONFIG_PINCTRL_LIB_DEVICE_NAME_PREFIX)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_DEVICE_NAME_PREFIX required!"
#endif
#if !defined(CONFIG_PINCTRL_LIB_CONFIG_GET)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_CONFIG_GET required!"
#endif
#if !defined(CONFIG_PINCTRL_LIB_CONFIG_SET)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_CONFIG_SET required!"
#endif
#if !defined(CONFIG_PINCTRL_LIB_MUX_GET)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_MUX_GET required!"
#endif
#if !defined(CONFIG_PINCTRL_LIB_MUX_SET)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_MUX_SET required!"
#endif
#if CONFIG_PINCTRL_LIB_SELECT_PINMUX && \
	!defined(CONFIG_PINCTRL_LIB_PINCONF_PINMUX_MUX)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_PINCONF_PINMUX_MUX required!"
#endif
#if CONFIG_PINCTRL_LIB_SELECT_PINMUX && \
	!defined(CONFIG_PINCTRL_LIB_PINCONF_PINMUX_PIN)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_PINCONF_PINMUX_PIN required!"
#endif
#if !defined(CONFIG_PINCTRL_LIB_CONTROLLER_COUNT)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_CONTROLLER_COUNT required!"
#endif
#if CONFIG_PINCTRL_LIB_CONTROLLER_COUNT > 4
#error "PINCTRL Lib:  4 pin controllers maximum!"
#endif
#if CONFIG_PINCTRL_LIB_CONTROLLER_COUNT > 0
#if !defined(CONFIG_PINCTRL_LIB_CONTROLLER_0_PREFIX)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_CONTROLLER_0_PREFIX required!"
#endif
#if !defined(CONFIG_PINCTRL_LIB_CONTROLLER_0_DATA)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_CONTROLLER_0_DATA required!"
#endif
#if !defined(CONFIG_PINCTRL_LIB_CONTROLLER_0_PIN_COUNT)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_CONTROLLER_0_PIN_COUNT required!"
#endif
#if !defined(CONFIG_PINCTRL_LIB_CONTROLLER_0_DEVICE_INIT)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_CONTROLLER_0_DEVICE_INIT required!"
#endif
#endif
#if CONFIG_PINCTRL_LIB_CONTROLLER_COUNT > 1
#if !defined(CONFIG_PINCTRL_LIB_CONTROLLER_1_PREFIX)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_CONTROLLER_1_PREFIX required!"
#endif
#if !defined(CONFIG_PINCTRL_LIB_CONTROLLER_1_DATA)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_CONTROLLER_1_DATA required!"
#endif
#if !defined(CONFIG_PINCTRL_LIB_CONTROLLER_1_PIN_COUNT)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_CONTROLLER_1_PIN_COUNT required!"
#endif
#if !defined(CONFIG_PINCTRL_LIB_CONTROLLER_1_DEVICE_INIT)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_CONTROLLER_1_DEVICE_INIT required!"
#endif
#endif
#if CONFIG_PINCTRL_LIB_CONTROLLER_COUNT > 2
#if !defined(CONFIG_PINCTRL_LIB_CONTROLLER_2_PREFIX)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_CONTROLLER_2_PREFIX required!"
#endif
#if !defined(CONFIG_PINCTRL_LIB_CONTROLLER_2_DATA)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_CONTROLLER_2_DATA required!"
#endif
#if !defined(CONFIG_PINCTRL_LIB_CONTROLLER_2_PIN_COUNT)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_CONTROLLER_2_PIN_COUNT required!"
#endif
#if !defined(CONFIG_PINCTRL_LIB_CONTROLLER_2_DEVICE_INIT)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_CONTROLLER_2_DEVICE_INIT required!"
#endif
#endif
#if CONFIG_PINCTRL_LIB_CONTROLLER_COUNT > 3
#if !defined(CONFIG_PINCTRL_LIB_CONTROLLER_3_PREFIX)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_CONTROLLER_3_PREFIX required!"
#endif
#if !defined(CONFIG_PINCTRL_LIB_CONTROLLER_3_DATA)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_CONTROLLER_3_DATA required!"
#endif
#if !defined(CONFIG_PINCTRL_LIB_CONTROLLER_3_PIN_COUNT)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_CONTROLLER_3_PIN_COUNT required!"
#endif
#if !defined(CONFIG_PINCTRL_LIB_CONTROLLER_3_DEVICE_INIT)
#error "PINCTRL Lib: CONFIG_PINCTRL_LIB_CONTROLLER_3_DEVICE_INIT required!"
#endif
#endif

#if DOXYGEN

/* Stop doxygen complaining about unknown defines */
#define CONFIG_PINCTRL_LIB_SELECT_PINMUX
#define CONFIG_PINCTRL_LIB_SELECT_GROUP
#define CONFIG_PINCTRL_LIB_SELECT_PINS
#define CONFIG_PINCTRL_LIB_DEVICE_NAME_PREFIX
#define CONFIG_PINCTRL_LIB_CONFIG_GET
#define CONFIG_PINCTRL_LIB_CONFIG_SET
#define CONFIG_PINCTRL_LIB_MUX_GET
#define CONFIG_PINCTRL_LIB_MUX_SET
#define CONFIG_PINCTRL_LIB_PINCONF_PINMUX_PIN
#define CONFIG_PINCTRL_LIB_PINCONF_PINMUX_MUX
#define CONFIG_PINCTRL_LIB_CONTROLLER_COUNT
#define CONFIG_PINCTRL_LIB_CONTROLLER_PIN_COUNT
#define CONFIG_PINCTRL_LIB_CONTROLLER_0_PREFIX
#define CONFIG_PINCTRL_LIB_CONTROLLER_0_DATA
#define CONFIG_PINCTRL_LIB_CONTROLLER_0_PIN_COUNT
#define CONFIG_PINCTRL_LIB_CONTROLLER_0_DEVICE_INIT
#define CONFIG_PINCTRL_LIB_CONTROLLER_1_PREFIX
#define CONFIG_PINCTRL_LIB_CONTROLLER_1_DATA
#define CONFIG_PINCTRL_LIB_CONTROLLER_1_PIN_COUNT
#define CONFIG_PINCTRL_LIB_CONTROLLER_1_DEVICE_INIT
#define CONFIG_PINCTRL_LIB_CONTROLLER_2_PREFIX
#define CONFIG_PINCTRL_LIB_CONTROLLER_2_DATA
#define CONFIG_PINCTRL_LIB_CONTROLLER_2_PIN_COUNT
#define CONFIG_PINCTRL_LIB_CONTROLLER_2_DEVICE_INIT
#define CONFIG_PINCTRL_LIB_CONTROLLER_3_PREFIX
#define CONFIG_PINCTRL_LIB_CONTROLLER_3_DATA
#define CONFIG_PINCTRL_LIB_CONTROLLER_3_PIN_COUNT
#define CONFIG_PINCTRL_LIB_CONTROLLER_3_DEVICE_INIT
#endif /* DOXYGEN */

#include <device.h>
#include <gpio.h>
#include <pinctrl.h>

#include <chaos/preprocessor/cat.h>
#include <chaos/preprocessor/control/when.h>
#include <chaos/preprocessor/repetition/repeat_from_to.h>

/*
 * Driver Settings
 */

/* PINCTRL_LIB_DEVICE_INIT */
#define PINCTRL_LIB_DEVICE_INIT CONFIG_PINCTRL_LIB_DEVICE_INIT

/* PINCTRL_LIB_CONFIG_GET */
#define PINCTRL_LIB_CONFIG_GET CONFIG_PINCTRL_LIB_CONFIG_GET

/* PINCTRL_LIB_CONFIG_SET */
#define PINCTRL_LIB_CONFIG_SET CONFIG_PINCTRL_LIB_CONFIG_SET

/* PINCTRL_LIB_MUX_GET */
#define PINCTRL_LIB_MUX_GET CONFIG_PINCTRL_LIB_MUX_GET

/* PINCTRL_LIB_MUX_SET */
#define PINCTRL_LIB_MUX_SET CONFIG_PINCTRL_LIB_MUX_SET

/* PINCTRL_LIB_PINCONF_PINMUX_PIN */
#define PINCTRL_LIB_PINCONF_PINMUX_PIN(pinmux) \
	CONFIG_PINCTRL_LIB_PINCONF_PINMUX_PIN(pinmux)

/* PINCTRL_LIB_PINCONF_PINMUX_MUX */
#define PINCTRL_LIB_PINCONF_PINMUX_MUX(pinmux) \
	CONFIG_PINCTRL_LIB_PINCONF_PINMUX_MUX(pinmux)

/* PINCTRL_LIB_CONTROLLER_COUNT */
#define PINCTRL_LIB_CONTROLLER_COUNT CONFIG_PINCTRL_LIB_CONTROLLER_COUNT

/* PINCTRL_LIB_CONTROLLER(idx) */
#define PINCTRL_LIB_CONTROLLER(idx) _PINCTRL_LIB_CONTROLLER1(idx)
#define _PINCTRL_LIB_CONTROLLER1(idx) \
	_PINCTRL_LIB_CONTROLLER2(CONFIG_PINCTRL_LIB_CONTROLLER_##idx##_PREFIX)
#define _PINCTRL_LIB_CONTROLLER2(_prefix) _PINCTRL_LIB_CONTROLLER3(_prefix)
#define _PINCTRL_LIB_CONTROLLER3(_prefix) _prefix

/* PINCTRL_LIB_CONTROLLER_DEVICE_NAME(idx) */
#define PINCTRL_LIB_CONTROLLER_DEVICE_NAME(idx) \
	_PINCTRL_LIB_CONTROLLER_DEVICE_NAME1(   \
		idx, CONFIG_PINCTRL_LIB_DEVICE_NAME_PREFIX)
#define _PINCTRL_LIB_CONTROLLER_DEVICE_NAME1(idx, _prefix) \
	_PINCTRL_LIB_CONTROLLER_DEVICE_NAME2(idx, _prefix)
#define _PINCTRL_LIB_CONTROLLER_DEVICE_NAME2(idx, _prefix) \
	_prefix##_##idx

/* PINCTRL_LIB_CONTROLLER_DRIVER_NAME(idx) */
#define PINCTRL_LIB_CONTROLLER_DRIVER_NAME(idx) \
	_PINCTRL_LIB_CONTROLLER_DRIVER_NAME1(idx)
#define _PINCTRL_LIB_CONTROLLER_DRIVER_NAME1(idx) \
	_PINCTRL_LIB_CONTROLLER_DRIVER_NAME2(PINCTRL_LIB_CONTROLLER(idx))
#define _PINCTRL_LIB_CONTROLLER_DRIVER_NAME2(_prefix) \
	_PINCTRL_LIB_CONTROLLER_DRIVER_NAME3(_prefix)
#define _PINCTRL_LIB_CONTROLLER_DRIVER_NAME3(_prefix) \
	_PINCTRL_LIB_CONTROLLER_DRIVER_NAME4(_prefix##_LABEL)
#define _PINCTRL_LIB_CONTROLLER_DRIVER_NAME4(_label) _label

/* PINCTRL_LIB_CONTROLLER_PIN_COUNT(idx) */
#define PINCTRL_LIB_CONTROLLER_PIN_COUNT(idx) \
	_PINCTRL_LIB_CONTROLLER_PIN_COUNT1(idx)
#define _PINCTRL_LIB_CONTROLLER_PIN_COUNT1(idx) \
	_PINCTRL_LIB_CONTROLLER_PIN_COUNT2(     \
		CONFIG_PINCTRL_LIB_CONTROLLER_##idx##_PIN_COUNT)
#define _PINCTRL_LIB_CONTROLLER_PIN_COUNT2(_count) _count

/* PINCTRL_LIB_CONTROLLER_DATA(idx) */
#define PINCTRL_LIB_CONTROLLER_DATA(idx) _PINCTRL_LIB_CONTROLLER_DATA1(idx)
#define _PINCTRL_LIB_CONTROLLER_DATA1(idx) \
	_PINCTRL_LIB_CONTROLLER_DATA2(     \
		CONFIG_PINCTRL_LIB_CONTROLLER_##idx##_DATA)
#define _PINCTRL_LIB_CONTROLLER_DATA2(_data) _data

/* PINCTRL_LIB_CONTROLLER_CONFIG_INFO(idx) */
#define PINCTRL_LIB_CONTROLLER_CONFIG_INFO(idx) \
	_PINCTRL_LIB_CONTROLLER_CONFIG_INFO1(idx)
#define _PINCTRL_LIB_CONTROLLER_CONFIG_INFO1(idx) \
	_PINCTRL_LIB_CONTROLLER_CONFIG_INFO2(     \
		PINCTRL_LIB_CONTROLLER_DEVICE_NAME(idx))
#define _PINCTRL_LIB_CONTROLLER_CONFIG_INFO2(_device_name) \
	_PINCTRL_LIB_CONTROLLER_CONFIG_INFO3(_device_name)
#define _PINCTRL_LIB_CONTROLLER_CONFIG_INFO3(_device_name) \
	_PINCTRL_LIB_CONTROLLER_CONFIG_INFO4(_device_name##_config)
#define _PINCTRL_LIB_CONTROLLER_CONFIG_INFO4(_config_info) _config_info

/* PINCTRL_LIB_CONTROLLER_DEVICE_INIT(idx) */
#define PINCTRL_LIB_CONTROLLER_DEVICE_INIT(idx) \
	_PINCTRL_LIB_CONTROLLER_DEVICE_INIT1(idx)
#define _PINCTRL_LIB_CONTROLLER_DEVICE_INIT1(idx) \
	_PINCTRL_LIB_CONTROLLER_DEVICE_INIT2(     \
		CONFIG_PINCTRL_LIB_CONTROLLER_##idx##_DEVICE_INIT)
#define _PINCTRL_LIB_CONTROLLER_DEVICE_INIT2(_device_init) _device_init

/* PINCTRL_LIB_CONTROLLER_DEVICE_INIT_PRIORITY(idx) */
#define PINCTRL_LIB_CONTROLLER_DEVICE_INIT_PRIORITY(idx) \
	CONFIG_PINCTRL_INIT_PRIORITY

/* PINCTRL_LIB_CONTROLLER_FUNCTION_COUNT(idx) */
#define PINCTRL_LIB_CONTROLLER_FUNCTION_COUNT(idx) \
	_PINCTRL_LIB_CONTROLLER_FUNCTION_COUNT1(idx)
#define _PINCTRL_LIB_CONTROLLER_FUNCTION_COUNT1(idx) \
	_PINCTRL_LIB_CONTROLLER_FUNCTION_COUNT2(PINCTRL_LIB_CONTROLLER(idx))
#define _PINCTRL_LIB_CONTROLLER_FUNCTION_COUNT2(_prefix) \
	_PINCTRL_LIB_CONTROLLER_FUNCTION_COUNT3(_prefix)
#define _PINCTRL_LIB_CONTROLLER_FUNCTION_COUNT3(_prefix) \
	_PINCTRL_LIB_CONTROLLER_FUNCTION_COUNT4(_prefix##_FUNCTION_COUNT)
#define _PINCTRL_LIB_CONTROLLER_FUNCTION_COUNT4(_count) _count

/* PINCTRL_LIB_CONTROLLER_STATE_NAME_COUNT(idx) */
#define PINCTRL_LIB_CONTROLLER_STATE_NAME_COUNT(idx) \
	_PINCTRL_LIB_CONTROLLER_STATE_NAME_COUNT1(idx)
#define _PINCTRL_LIB_CONTROLLER_STATE_NAME_COUNT1(idx) \
	_PINCTRL_LIB_CONTROLLER_STATE_NAME_COUNT2(PINCTRL_LIB_CONTROLLER(idx))
#define _PINCTRL_LIB_CONTROLLER_STATE_NAME_COUNT2(_prefix) \
	_PINCTRL_LIB_CONTROLLER_STATE_NAME_COUNT3(_prefix)
#define _PINCTRL_LIB_CONTROLLER_STATE_NAME_COUNT3(_prefix) \
	_PINCTRL_LIB_CONTROLLER_STATE_NAME_COUNT4(_prefix##_STATE_NAME_COUNT)
#define _PINCTRL_LIB_CONTROLLER_STATE_NAME_COUNT4(_count) _count

/* PINCTRL_LIB_CONTROLLER_PINCTRL_STATE_COUNT(idx) */
#define PINCTRL_LIB_CONTROLLER_PINCTRL_STATE_COUNT(idx) \
	_PINCTRL_LIB_CONTROLLER_PINCTRL_STATE_COUNT1(idx)
#define _PINCTRL_LIB_CONTROLLER_PINCTRL_STATE_COUNT1(idx) \
	_PINCTRL_LIB_CONTROLLER_PINCTRL_STATE_COUNT2(     \
		PINCTRL_LIB_CONTROLLER(idx))
#define _PINCTRL_LIB_CONTROLLER_PINCTRL_STATE_COUNT2(_prefix) \
	_PINCTRL_LIB_CONTROLLER_PINCTRL_STATE_COUNT3(_prefix)
#define _PINCTRL_LIB_CONTROLLER_PINCTRL_STATE_COUNT3(_prefix) \
	_PINCTRL_LIB_CONTROLLER_PINCTRL_STATE_COUNT4(         \
		_prefix##_PINCTRL_STATE_COUNT)
#define _PINCTRL_LIB_CONTROLLER_PINCTRL_STATE_COUNT4(_count) _count

/* PINCTRL_LIB_CONTROLLER_PINCTRL_COUNT(idx) */
#define PINCTRL_LIB_CONTROLLER_PINCTRL_COUNT(idx) \
	_PINCTRL_LIB_CONTROLLER_PINCTRL_COUNT1(idx)
#define _PINCTRL_LIB_CONTROLLER_PINCTRL_COUNT1(idx) \
	_PINCTRL_LIB_CONTROLLER_PINCTRL_COUNT2(PINCTRL_LIB_CONTROLLER(idx))
#define _PINCTRL_LIB_CONTROLLER_PINCTRL_COUNT2(_prefix) \
	_PINCTRL_LIB_CONTROLLER_PINCTRL_COUNT3(_prefix)
#define _PINCTRL_LIB_CONTROLLER_PINCTRL_COUNT3(_prefix) \
	_PINCTRL_LIB_CONTROLLER_PINCTRL_COUNT4(_prefix##_PINCTRL_COUNT)
#define _PINCTRL_LIB_CONTROLLER_PINCTRL_COUNT4(_count) _count

/* PINCTRL_LIB_CONTROLLER_GPIO_RANGE_COUNT(idx) */
#define PINCTRL_LIB_CONTROLLER_GPIO_RANGE_COUNT(idx) \
	_PINCTRL_LIB_CONTROLLER_GPIO_RANGE_COUNT1(idx)
#define _PINCTRL_LIB_CONTROLLER_GPIO_RANGE_COUNT1(idx) \
	_PINCTRL_LIB_CONTROLLER_GPIO_RANGE_COUNT2(PINCTRL_LIB_CONTROLLER(idx))
#define _PINCTRL_LIB_CONTROLLER_GPIO_RANGE_COUNT2(_prefix) \
	_PINCTRL_LIB_CONTROLLER_GPIO_RANGE_COUNT3(_prefix)
#define _PINCTRL_LIB_CONTROLLER_GPIO_RANGE_COUNT3(_prefix) \
	_PINCTRL_LIB_CONTROLLER_GPIO_RANGE_COUNT4(_prefix##_GPIO_RANGE_COUNT)
#define _PINCTRL_LIB_CONTROLLER_GPIO_RANGE_COUNT4(_count) _count

/**
 * @def PINCTRL_LIB_FUNCTION
 * @brief Helper for function info initialization.
 *
 * Creates an initialization for a function info with index idx.
 *
 * @param idx Function info index
 * @param _controller_idx Controller index [0..3]
 */
#define PINCTRL_LIB_FUNCTION(idx, _controller_idx) \
	_PINCTRL_LIB_FUNCTION1(idx, _controller_idx)
#define _PINCTRL_LIB_FUNCTION1(idx, _controller_idx) \
	_PINCTRL_LIB_FUNCTION2(idx, PINCTRL_LIB_CONTROLLER(_controller_idx))
#define _PINCTRL_LIB_FUNCTION2(idx, _prefix) \
	_PINCTRL_LIB_FUNCTION3(idx, _prefix)
#define _PINCTRL_LIB_FUNCTION3(idx, _prefix) \
	_PINCTRL_LIB_FUNCTION4(_prefix##_FUNCTION_##idx##_CLIENT)
#define _PINCTRL_LIB_FUNCTION4(_client_define_label) \
	_PINCTRL_LIB_FUNCTION5(_client_define_label)
#define _PINCTRL_LIB_FUNCTION5(_client_define_label) \
	_PINCTRL_LIB_FUNCTION6(_client_define_label##_LABEL)
#define _PINCTRL_LIB_FUNCTION6(_name) _PINCTRL_LIB_FUNCTION7(_name)
#define _PINCTRL_LIB_FUNCTION7(_name) \
	{                             \
		.name = _name,        \
	},

/* Macros for CHAOS_PP_REPEAT_FROM_TO with signature macro(s,n) */
#define PINCTRL_LIB_CONTROLLER_0_FUNCTION(s, n) PINCTRL_LIB_FUNCTION(n, 0)
#define PINCTRL_LIB_CONTROLLER_1_FUNCTION(s, n) PINCTRL_LIB_FUNCTION(n, 1)
#define PINCTRL_LIB_CONTROLLER_2_FUNCTION(s, n) PINCTRL_LIB_FUNCTION(n, 2)
#define PINCTRL_LIB_CONTROLLER_3_FUNCTION(s, n) PINCTRL_LIB_FUNCTION(n, 3)

/* Get macro for CHAOS_PP_REPEAT_FROM_TO */
#define PINCTRL_LIB_CONTROLLER_REPEAT_FUNCTION(controller_idx) \
	_PINCTRL_LIB_CONTROLLER_REPEAT_FUNCTION1(controller_idx)
#define _PINCTRL_LIB_CONTROLLER_REPEAT_FUNCTION1(controller_idx) \
	_PINCTRL_LIB_CONTROLLER_REPEAT_FUNCTION2(                \
		PINCTRL_LIB_CONTROLLER_##controller_idx##_FUNCTION)
#define _PINCTRL_LIB_CONTROLLER_REPEAT_FUNCTION2(_repeat_function) \
	_repeat_function

/**
 * @def PINCTRL_LIB_STATE_NAME
 * @brief Helper for pinctrl state name initialization.
 *
 * Creates an initialization for a pinctrl state name with index idx.
 *
 * @param idx State name info index
 * @param _controller_idx Controller index [0..3]
 */
#define PINCTRL_LIB_STATE_NAME(idx, _controller_idx) \
	_PINCTRL_LIB_STATE_NAME1(idx, _controller_idx)
#define _PINCTRL_LIB_STATE_NAME1(idx, _controller_idx) \
	_PINCTRL_LIB_STATE_NAME2(idx, PINCTRL_LIB_CONTROLLER(_controller_idx))
#define _PINCTRL_LIB_STATE_NAME2(idx, _prefix) \
	_PINCTRL_LIB_STATE_NAME3(idx, _prefix)
#define _PINCTRL_LIB_STATE_NAME3(idx, _prefix) \
	_PINCTRL_LIB_STATE_NAME4(_prefix##_STATE_NAME_##idx)
#define _PINCTRL_LIB_STATE_NAME4(_name) _PINCTRL_LIB_STATE_NAME5(_name)
#define _PINCTRL_LIB_STATE_NAME5(_name) \
	{                               \
		.name = #_name,         \
	},

/* Macros for CHAOS_PP_REPEAT_FROM_TO with signature macro(s,n) */
#define PINCTRL_LIB_CONTROLLER_0_STATE_NAME(s, n) PINCTRL_LIB_STATE_NAME(n, 0)
#define PINCTRL_LIB_CONTROLLER_1_STATE_NAME(s, n) PINCTRL_LIB_STATE_NAME(n, 1)
#define PINCTRL_LIB_CONTROLLER_2_STATE_NAME(s, n) PINCTRL_LIB_STATE_NAME(n, 2)
#define PINCTRL_LIB_CONTROLLER_3_STATE_NAME(s, n) PINCTRL_LIB_STATE_NAME(n, 3)

/* Get macro for CHAOS_PP_REPEAT_FROM_TO */
#define PINCTRL_LIB_CONTROLLER_REPEAT_STATE_NAME(controller_idx) \
	_PINCTRL_LIB_CONTROLLER_REPEAT_STATE_NAME1(controller_idx)
#define _PINCTRL_LIB_CONTROLLER_REPEAT_STATE_NAME1(controller_idx) \
	_PINCTRL_LIB_CONTROLLER_REPEAT_STATE_NAME2(                \
		PINCTRL_LIB_CONTROLLER_##controller_idx##_STATE_NAME)
#define _PINCTRL_LIB_CONTROLLER_REPEAT_STATE_NAME2(_repeat_function) \
	_repeat_function

/**
 * @def PINCTRL_LIB_PINCTRL_STATE
 * @brief Helper for pinctrl state initialization.
 *
 * Creates an initialization for a pinctrl state with index idx.
 *
 * @param idx State name info index
 * @param _controller_idx Controller index [0..3]
 */
#define PINCTRL_LIB_PINCTRL_STATE(idx, _controller_idx) \
	_PINCTRL_LIB_PINCTRL_STATE1(idx, _controller_idx)
#define _PINCTRL_LIB_PINCTRL_STATE1(idx, _controller_idx) \
	_PINCTRL_LIB_PINCTRL_STATE2(idx,                  \
				    PINCTRL_LIB_CONTROLLER(_controller_idx))
#define _PINCTRL_LIB_PINCTRL_STATE2(idx, _prefix) \
	_PINCTRL_LIB_PINCTRL_STATE3(idx, _prefix)
#define _PINCTRL_LIB_PINCTRL_STATE3(idx, _prefix)        \
	_PINCTRL_LIB_PINCTRL_STATE4(                     \
		_prefix##_PINCTRL_STATE_##idx##_NAME_ID, \
		_prefix##_PINCTRL_STATE_##idx##_FUNCTION_ID)
#define _PINCTRL_LIB_PINCTRL_STATE4(_name_id, _function_id) \
	_PINCTRL_LIB_PINCTRL_STATE5(_name_id, _function_id)
#define _PINCTRL_LIB_PINCTRL_STATE5(_name_id, _function_id) \
	{                                                   \
		.function = _function_id, .name = _name_id, \
	},

/* Macros for CHAOS_PP_REPEAT_FROM_TO with signature macro(s,n) */
#define PINCTRL_LIB_CONTROLLER_0_PINCTRL_STATE(s, n) \
	PINCTRL_LIB_PINCTRL_STATE(n, 0)
#define PINCTRL_LIB_CONTROLLER_1_PINCTRL_STATE(s, n) \
	PINCTRL_LIB_PINCTRL_STATE(n, 1)
#define PINCTRL_LIB_CONTROLLER_2_PINCTRL_STATE(s, n) \
	PINCTRL_LIB_PINCTRL_STATE(n, 2)
#define PINCTRL_LIB_CONTROLLER_3_PINCTRL_STATE(s, n) \
	PINCTRL_LIB_PINCTRL_STATE(n, 3)

/* Get macro for CHAOS_PP_REPEAT_FROM_TO */
#define PINCTRL_LIB_CONTROLLER_REPEAT_PINCTRL_STATE(controller_idx) \
	_PINCTRL_LIB_CONTROLLER_REPEAT_PINCTRL_STATE1(controller_idx)
#define _PINCTRL_LIB_CONTROLLER_REPEAT_PINCTRL_STATE1(controller_idx) \
	_PINCTRL_LIB_CONTROLLER_REPEAT_PINCTRL_STATE2(                \
		PINCTRL_LIB_CONTROLLER_##controller_idx##_PINCTRL_STATE)
#define _PINCTRL_LIB_CONTROLLER_REPEAT_PINCTRL_STATE2(_repeat_function) \
	_repeat_function

/**
 * @def PINCTRL_LIB_PINCTRL
 * @brief Helper for pin control initialisation.
 *
 * Creates an initialization for pin control with index idx.
 *
 * @param idx Pinctrl info index
 * @param _controller_idx Controller index [0..3]
 */
#define PINCTRL_LIB_PINCTRL(idx, _controller_idx) \
	_PINCTRL_LIB_PINCTRL1(idx, _controller_idx)
#define _PINCTRL_LIB_PINCTRL1(idx, _controller_idx) \
	_PINCTRL_LIB_PINCTRL2(idx, PINCTRL_LIB_CONTROLLER(_controller_idx))
#define _PINCTRL_LIB_PINCTRL2(idx, _prefix) _PINCTRL_LIB_PINCTRL3(idx, _prefix)
#define _PINCTRL_LIB_PINCTRL3(idx, _prefix)                                    \
	_PINCTRL_LIB_PINCTRL4(PINCTRL_LIB_PINCONF_PINMUX_PIN(                  \
				      _prefix##_PINCTRL_##idx##_PINMUX),       \
			      PINCTRL_LIB_PINCONF_PINMUX_MUX(                  \
				      _prefix##_PINCTRL_##idx##_PINMUX),       \
			      _prefix##_PINCTRL_##idx##_BIAS_DISABLE,          \
			      _prefix##_PINCTRL_##idx##_BIAS_HIGH_IMPEDANCE,   \
			      _prefix##_PINCTRL_##idx##_BIAS_BUS_HOLD,         \
			      _prefix##_PINCTRL_##idx##_BIAS_PULL_UP,          \
			      _prefix##_PINCTRL_##idx##_BIAS_PULL_DOWN,        \
			      _prefix##_PINCTRL_##idx##_BIAS_PULL_PIN_DEFAULT, \
			      _prefix##_PINCTRL_##idx##_DRIVE_PUSH_PULL,       \
			      _prefix##_PINCTRL_##idx##_DRIVE_OPEN_DRAIN,      \
			      _prefix##_PINCTRL_##idx##_DRIVE_OPEN_SOURCE,     \
			      _prefix##_PINCTRL_##idx##_DRIVE_STRENGTH,        \
			      _prefix##_PINCTRL_##idx##_INPUT_ENABLE,          \
			      _prefix##_PINCTRL_##idx##_INPUT_DISABLE,         \
			      _prefix##_PINCTRL_##idx##_INPUT_SCHMITT_ENABLE,  \
			      _prefix##_PINCTRL_##idx##_INPUT_SCHMITT_DISABLE, \
			      _prefix##_PINCTRL_##idx##_INPUT_DEBOUNCE,        \
			      _prefix##_PINCTRL_##idx##_POWER_SOURCE,          \
			      _prefix##_PINCTRL_##idx##_LOW_POWER_ENABLE,      \
			      _prefix##_PINCTRL_##idx##_LOW_POWER_DISABLE,     \
			      _prefix##_PINCTRL_##idx##_OUTPUT_ENABLE,         \
			      _prefix##_PINCTRL_##idx##_OUTPUT_DISABLE,        \
			      _prefix##_PINCTRL_##idx##_OUTPUT_LOW,            \
			      _prefix##_PINCTRL_##idx##_OUTPUT_HIGH,           \
			      _prefix##_PINCTRL_##idx##_SLEW_RATE,             \
			      _prefix##_PINCTRL_##idx##_SPEED,                 \
			      _prefix##_PINCTRL_##idx##_STATE_ID)
#define _PINCTRL_LIB_PINCTRL4(_pin,                   \
			      _mux,                   \
			      _bias_disable,          \
			      _bias_high_impedance,   \
			      _bias_bus_hold,         \
			      _bias_pull_up,          \
			      _bias_pull_down,        \
			      _bias_pull_pin_default, \
			      _drive_push_pull,       \
			      _drive_open_drain,      \
			      _drive_open_source,     \
			      _drive_strength,        \
			      _input_enable,          \
			      _input_disable,         \
			      _input_schmitt_enable,  \
			      _input_schmitt_disable, \
			      _input_debounce,        \
			      _power_source,          \
			      _low_power_enable,      \
			      _low_power_disable,     \
			      _output_enable,         \
			      _output_disable,        \
			      _output_low,            \
			      _output_high,           \
			      _slew_rate,             \
			      _speed,                 \
			      _state)                 \
	_PINCTRL_LIB_PINCTRL5(_pin,                   \
			      _mux,                   \
			      _bias_disable,          \
			      _bias_high_impedance,   \
			      _bias_bus_hold,         \
			      _bias_pull_up,          \
			      _bias_pull_down,        \
			      _bias_pull_pin_default, \
			      _drive_push_pull,       \
			      _drive_open_drain,      \
			      _drive_open_source,     \
			      _drive_strength,        \
			      _input_enable,          \
			      _input_disable,         \
			      _input_schmitt_enable,  \
			      _input_schmitt_disable, \
			      _input_debounce,        \
			      _power_source,          \
			      _low_power_enable,      \
			      _low_power_disable,     \
			      _output_enable,         \
			      _output_disable,        \
			      _output_low,            \
			      _output_high,           \
			      _slew_rate,             \
			      _speed,                 \
			      _state)
#define _PINCTRL_LIB_PINCTRL5(_pin,                                           \
			      _mux,                                           \
			      _bias_disable,                                  \
			      _bias_high_impedance,                           \
			      _bias_bus_hold,                                 \
			      _bias_pull_up,                                  \
			      _bias_pull_down,                                \
			      _bias_pull_pin_default,                         \
			      _drive_push_pull,                               \
			      _drive_open_drain,                              \
			      _drive_open_source,                             \
			      _drive_strength,                                \
			      _input_enable,                                  \
			      _input_disable,                                 \
			      _input_schmitt_enable,                          \
			      _input_schmitt_disable,                         \
			      _input_debounce,                                \
			      _power_source,                                  \
			      _low_power_enable,                              \
			      _low_power_disable,                             \
			      _output_enable,                                 \
			      _output_disable,                                \
			      _output_low,                                    \
			      _output_high,                                   \
			      _slew_rate,                                     \
			      _speed,                                         \
			      _state)                                         \
	{                                                                     \
		.pin = _pin,                                                  \
		.mux = _mux,                                                  \
		.state = _state,                                              \
		.config =                                                     \
			(_bias_disable * PINCTRL_CONFIG_BIAS_DISABLE) |       \
			(_bias_high_impedance *                               \
			 PINCTRL_CONFIG_BIAS_HIGH_IMPEDANCE) |                \
			(_bias_bus_hold * PINCTRL_CONFIG_BIAS_BUS_HOLD) |     \
			(_bias_pull_up * PINCTRL_CONFIG_BIAS_PULL_UP) |       \
			(_bias_pull_down * PINCTRL_CONFIG_BIAS_PULL_DOWN) |   \
			(_bias_pull_pin_default *                             \
			 PINCTRL_CONFIG_BIAS_PULL_PIN_DEFAULT) |              \
			(_drive_push_pull * PINCTRL_CONFIG_DRIVE_PUSH_PULL) | \
			(_drive_open_drain *                                  \
			 PINCTRL_CONFIG_DRIVE_OPEN_DRAIN) |                   \
			(_drive_open_source *                                 \
			 PINCTRL_CONFIG_DRIVE_OPEN_SOURCE) |                  \
			(_drive_strength & PINCTRL_CONFIG_DRIVE_STRENGTH_7) | \
			(_input_enable * PINCTRL_CONFIG_INPUT_ENABLE) |       \
			(_input_disable * PINCTRL_CONFIG_INPUT_DISABLE) |     \
			(_input_schmitt_enable *                              \
			 PINCTRL_CONFIG_INPUT_SCHMITT_ENABLE) |               \
			(_input_schmitt_disable *                             \
			 PINCTRL_CONFIG_INPUT_SCHMITT_DISABLE) |              \
			(_input_debounce &                                    \
			 PINCTRL_CONFIG_INPUT_DEBOUNCE_LONG) |                \
			(_power_source & PINCTRL_CONFIG_POWER_SOURCE_3) |     \
			(_low_power_enable *                                  \
			 PINCTRL_CONFIG_LOW_POWER_ENABLE) |                   \
			(_low_power_disable *                                 \
			 PINCTRL_CONFIG_LOW_POWER_DISABLE) |                  \
			(_output_enable * PINCTRL_CONFIG_OUTPUT_ENABLE) |     \
			(_output_disable * PINCTRL_CONFIG_OUTPUT_DISABLE) |   \
			(_output_low * PINCTRL_CONFIG_OUTPUT_LOW) |           \
			(_output_high * PINCTRL_CONFIG_OUTPUT_HIGH) |         \
			(_slew_rate & PINCTRL_CONFIG_SLEW_RATE_FAST) |        \
			(_speed & PINCTRL_CONFIG_SPEED_HIGH),                 \
	},

/* Macros for CHAOS_PP_REPEAT_FROM_TO with signature macro(s,n) */
#define PINCTRL_LIB_CONTROLLER_0_PINCTRL(s, n) PINCTRL_LIB_PINCTRL(n, 0)
#define PINCTRL_LIB_CONTROLLER_1_PINCTRL(s, n) PINCTRL_LIB_PINCTRL(n, 1)
#define PINCTRL_LIB_CONTROLLER_2_PINCTRL(s, n) PINCTRL_LIB_PINCTRL(n, 2)
#define PINCTRL_LIB_CONTROLLER_3_PINCTRL(s, n) PINCTRL_LIB_PINCTRL(n, 3)

/* Get macro for CHAOS_PP_REPEAT_FROM_TO */
#define PINCTRL_LIB_CONTROLLER_REPEAT_PINCTRL(controller_idx) \
	_PINCTRL_LIB_CONTROLLER_REPEAT_PINCTRL1(controller_idx)
#define _PINCTRL_LIB_CONTROLLER_REPEAT_PINCTRL1(controller_idx) \
	_PINCTRL_LIB_CONTROLLER_REPEAT_PINCTRL2(                \
		PINCTRL_LIB_CONTROLLER_##controller_idx##_PINCTRL)
#define _PINCTRL_LIB_CONTROLLER_REPEAT_PINCTRL2(_repeat_function) \
	_repeat_function

/**
 * @def PINCTRL_LIB_GPIO_RANGE
 * @brief Helper for gpio range initialization.
 *
 * Creates an initialization for a gpio range with index idx.
 *
 * @param idx GPIO range info index
 * @param _controller_idx Controller index [0..3]
 */
#define PINCTRL_LIB_GPIO_RANGE(idx, _controller_idx) \
	_PINCTRL_LIB_GPIO_RANGE1(idx, _controller_idx)
#define _PINCTRL_LIB_GPIO_RANGE1(idx, _controller_idx) \
	_PINCTRL_LIB_GPIO_RANGE2(idx, PINCTRL_LIB_CONTROLLER(_controller_idx))
#define _PINCTRL_LIB_GPIO_RANGE2(idx, _prefix) \
	_PINCTRL_LIB_GPIO_RANGE3(idx, _prefix)
#define _PINCTRL_LIB_GPIO_RANGE3(idx, _prefix)                             \
	_PINCTRL_LIB_GPIO_RANGE4(_prefix##_GPIO_RANGE_##idx##_CLIENT,      \
				 _prefix##_GPIO_RANGE_##idx##_CLIENT_BASE, \
				 _prefix##_GPIO_RANGE_##idx##_BASE,        \
				 _prefix##_GPIO_RANGE_##idx##_NPINS)
#define _PINCTRL_LIB_GPIO_RANGE4(_gpio_def_label, _gpio_base, _base, _npins) \
	_PINCTRL_LIB_GPIO_RANGE5(_gpio_def_label, _gpio_base, _base, _npins)
#define _PINCTRL_LIB_GPIO_RANGE5(_gpio_def_label, _gpio_base, _base, _npins) \
	_PINCTRL_LIB_GPIO_RANGE6(_gpio_def_label##_LABEL,                    \
				 GPIO_PORT_PIN_IDX(_gpio_base),              \
				 _base,                                      \
				 _npins)
#define _PINCTRL_LIB_GPIO_RANGE6(_name, _gpio_base_idx, _base, _npins) \
	_PINCTRL_LIB_GPIO_RANGE7(_name, _gpio_base_idx, _base, _npins)
#define _PINCTRL_LIB_GPIO_RANGE7(_name, _gpio_base_idx, _base, _npins) \
	{                                                              \
		.gpio_base_idx = _gpio_base_idx,                       \
		.npins = _npins,                                       \
		.base = _base,                                         \
		.name = _name,                                         \
	},

/* Macros for CHAOS_PP_REPEAT_FROM_TO with signature macro(s,n) */
#define PINCTRL_LIB_CONTROLLER_0_GPIO_RANGE(s, n) PINCTRL_LIB_GPIO_RANGE(n, 0)
#define PINCTRL_LIB_CONTROLLER_1_GPIO_RANGE(s, n) PINCTRL_LIB_GPIO_RANGE(n, 1)
#define PINCTRL_LIB_CONTROLLER_2_GPIO_RANGE(s, n) PINCTRL_LIB_GPIO_RANGE(n, 2)
#define PINCTRL_LIB_CONTROLLER_3_GPIO_RANGE(s, n) PINCTRL_LIB_GPIO_RANGE(n, 3)

/* Get macro for CHAOS_PP_REPEAT_FROM_TO */
#define PINCTRL_LIB_CONTROLLER_REPEAT_GPIO_RANGE(controller_idx) \
	_PINCTRL_LIB_CONTROLLER_REPEAT_GPIO_RANGE1(controller_idx)
#define _PINCTRL_LIB_CONTROLLER_REPEAT_GPIO_RANGE1(controller_idx) \
	_PINCTRL_LIB_CONTROLLER_REPEAT_GPIO_RANGE2(                \
		PINCTRL_LIB_CONTROLLER_##controller_idx##_GPIO_RANGE)
#define _PINCTRL_LIB_CONTROLLER_REPEAT_GPIO_RANGE2(_repeat_function) \
	_repeat_function

/**
 * @brief Function info.
 */
struct pinctrl_lib_function {
	const char *name;
};

/**
 * @brief State name info.
 */
struct pinctrl_lib_state_name {
	const char *name;
};

/**
 * @brief Pinctrl state info.
 */
struct pinctrl_lib_pinctrl_state {
	u16_t function;
	u16_t name;
};

/**
 * @brief Pinctrl info.
 */
struct pinctrl_lib_pinctrl {
	u16_t pin;		/* pin ID */
	u16_t mux;
	u16_t state;
	u32_t config;
};

/**
 * @brief GPIO range info.
 */
struct pinctrl_lib_gpio_range {
	u8_t gpio_base_idx;
	u8_t npins;
	u16_t base;
	const char *name;
};

/**
 * @brief Pin controller configuration info.
 */
struct pinctrl_lib_config {
	u16_t pin_count;
	u16_t function_count;
	u16_t state_name_count;
	u16_t pinctrl_state_count;
	u16_t pinctrl_count;
	u8_t gpio_range_count;
	const struct pinctrl_lib_function *function_data;
	const struct pinctrl_lib_state_name *state_name_data;
	const struct pinctrl_lib_pinctrl_state *pinctrl_state_data;
	const struct pinctrl_lib_pinctrl *pinctrl_data;
	const struct pinctrl_lib_gpio_range *gpio_range_data;
	u8_t *mux_pinmap;
	int (*device_init)(struct device *dev);
};

static inline const struct pinctrl_lib_config
	*pinctrl_lib_device_config_info(struct device *dev)
{
	return (const struct pinctrl_lib_config *) dev->config->config_info;
}

#if CONFIG_PINCTRL_LIB_ENABLE_MUX_REQUEST

/**
 * @brief Pin multiplex owner info
 */
struct pinctrl_lib_mux_owner {
	const char *name;
};

/**
 * @brief Pin multiplex owner data
 */
struct pinctrl_lib_mux_owner pinctrl_lib_mux_owner_data[15];

static const char *pinctrl_lib_mux_owner_get(struct device *dev, u16_t pin)
{
	const struct pinctrl_lib_config *config =
		pinctrl_lib_device_config_info(dev);

	int pinmap_idx = pin >> 1;
	u8_t owner_idx = config->mux_pinmap[pinmap_idx];

	if (pin & 0x01) {
		owner_idx = owner_idx >> 4;
	} else {
		owner_idx &= 0x0F;
	}
	if (owner_idx >= 15) {
		/* no owner */
		return 0;
	}
	return pinctrl_lib_mux_owner_data[owner_idx].name;
}

static int pinctrl_lib_mux_owner_set(struct device *dev, u16_t pin,
				     const char *owner)
{
	const struct pinctrl_lib_config *config =
		pinctrl_lib_device_config_info(dev);

	int pinmap_idx = pin >> 1;
	u8_t owner_idx;

	if (owner == 0) {
		/* No owner */
		owner_idx = 15;
	} else {
		for (owner_idx = 0; owner_idx < 15; owner_idx++) {
			const char *name =
				pinctrl_lib_mux_owner_data[owner_idx].name;
			if (name && (strcmp(name, owner) == 0)) {
				break;
			}
		}
		if (owner_idx >= 15) {
			/* owner unknown - add it */
			for (owner_idx = 0; owner_idx < 15; owner_idx++) {
				if (pinctrl_lib_mux_owner_data[owner_idx]
				    .name == 0) {
					pinctrl_lib_mux_owner_data[owner_idx]
						.name = owner;
					break;
				}
			}
		}
		if (owner_idx >= 15) {
			/* no more memory for owner info */
			return -ENOMEM;
		}
	}
	if (pin & 0x01) {
		config->mux_pinmap[pinmap_idx] =
			(config->mux_pinmap[pinmap_idx] & 0x0F) |
			(owner_idx << 4);
	} else {
		config->mux_pinmap[pinmap_idx] =
			(config->mux_pinmap[pinmap_idx] & 0xF0) | owner_idx;
	}
	return 0;
}

static void pinctrl_lib_mux_owner_init(void)
{
	for (int owner_idx = 0; owner_idx < 15; owner_idx++) {
		pinctrl_lib_mux_owner_data[owner_idx].name = 0;
	}
}

static int pinctrl_lib_mux_request_init(struct device *dev)
{
	static u8_t mux_owner_initialized; /* initialized to 0 */

	const struct pinctrl_lib_config *config =
		pinctrl_lib_device_config_info(dev);

	if (!mux_owner_initialized) {
		pinctrl_lib_mux_owner_init();
		mux_owner_initialized = 1;
	}
	for (int i = 0; i < config->pin_count / 2; i++) {
		config->mux_pinmap[i] = 0xFF;
	}
	return 0;
}

#endif /* CONFIG_PINCTRL_LIB_ENABLE_MUX_REQUEST */

/* --- API --- */

/**
 * Get the number of pins controlled by this pin controller.
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @return number of pins
 */
static u16_t pinctrl_lib_control_get_pins_count(struct device *dev)
{
	return pinctrl_lib_device_config_info(dev)->pin_count;
}

/**
 * Get the number of groups selectable by this pin controller.
 *
 * @note Every state is regarded a group. Resulting multi definition
 *       of the same set of pins is accepted for the sake of code simplicity.
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @return number of states
 */
static int pinctrl_lib_control_get_groups_count(struct device *dev)
{
	return pinctrl_lib_device_config_info(dev)->pinctrl_state_count;
}

/**
 * Get the pins that are in a pin group.
 *
 * Returns an array of pin numbers. The applicable pins
 * are returned in @p pins and the number of pins in @p num_pins. The
 * number of pins returned is bounded by the initial value of @p num_pins
 * when called.
 *
 * @note The pin group is defined by the pinctrl state.
 *       The group number equals the pinctrl state index.
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param group Group
 * @param[in,out] pins Array of pins.
 * @param[in,out] num_pins Size of the array of pins.
 * @retval 0 on success
 * @retval -EINVAL if number of pins exceeds array size.
 * @retval -ENOTSUP if requested group is not available on this controller
 */
static int pinctrl_lib_control_get_group_pins(struct device *dev, u16_t group,
					      u16_t *pins, u16_t *num_pins)
{
	const struct pinctrl_lib_config *config =
		pinctrl_lib_device_config_info(dev);

	u16_t num_pins_found = 0;

	for (int i = 0; i < config->pinctrl_count; i++) {
		if (config->pinctrl_data[i].state == group) {
			if (num_pins_found < *num_pins) {
				pins[num_pins_found] =
					config->pinctrl_data[i].pin;
			}
			num_pins_found++;
		}
	}
	if (num_pins_found == 0) {
		*num_pins = 0;
		return -ENOTSUP;
	}
	if (num_pins_found > *num_pins) {
		*num_pins = num_pins_found;
		return -EINVAL;
	}
	*num_pins = num_pins_found;
	return 0;
}

/**
 * Get the number of states selectable by this pin controller.
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @return number of states
 */
static u16_t pinctrl_lib_control_get_states_count(struct device *dev)
{
	return pinctrl_lib_device_config_info(dev)->pinctrl_state_count;
}

/**
 * Get the group of pins controlled by the pinctrl state.
 *
 * @note Every state is regarded a group. Resulting multi definition
 *       of the same set of pins is accepted for the sake of code simplicity.
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param state State
 * @param[out] group Group
 * @retval 0 on success
 * @retval -ENOTSUP if requested state is not available on this controller
 */
static int pinctrl_lib_control_get_state_group(struct device *dev,
					       u16_t state, u16_t *group)
{
	const struct pinctrl_lib_config *config =
		pinctrl_lib_device_config_info(dev);

	if (state >= config->pinctrl_state_count) {
		return -ENOTSUP;
	}
	*group = state;
	return 0;
}

/**
 * Get the number of selectable functions of this pin controller.
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @return number of functions
 */
static u16_t pinctrl_lib_control_get_functions_count(struct device *dev)
{
	return pinctrl_lib_device_config_info(dev)->function_count;
}

/**
 * Get the pin group of given name that is related to given function.
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param func Function
 * @param name Group name
 * @param[out] group Group
 * @retval 0 on success
 * @retval -ENODEV if requested function is not available on this controller
 * @retval -ENOTSUP if there is no group with the requested name for
 *                  the function
 */
static int pinctrl_lib_control_get_function_group(struct device *dev,
						  u16_t func,
						  const char *name,
						  u16_t *group)
{
	const struct pinctrl_lib_config *config =
		pinctrl_lib_device_config_info(dev);

	if (func >= config->function_count) {
		return -ENODEV;
	}

	for (u16_t i = 0; i < config->pinctrl_state_count; i++) {
		/* state name is name also used as group name */
		const char *group_name =
			config->state_name_data[config->pinctrl_state_data[i]
						.name].name;

		if ((func == config->pinctrl_state_data[i].function) &&
		    (strcmp(name, group_name) == 0)) {
			*group = i;
			return 0;
		}
	}
	return -ENOTSUP;
}

/**
 * Get the groups of pins that can be multiplexed to the function.
 *
 * Returns an array of group numbers. The group number can be used with
 * pinctrl_get_group_pins() to retrieve the pins. The applicable groups
 * are returned in @p groups and the number of groups in @p num_groups. The
 * number of groups returned is bounded by the initial value of @p num_groups
 * when called.
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param func Function
 * @param[out] groups Array of groups.
 * @param[in,out] num_groups Size of the array of groups.
 * @retval 0 on success
 * @retval -ENODEV if requested function is not available on this controller
 * @retval -ENOTSUP if there is no group
 * @retval -EINVAL if the number of available groups exceeds array size
 */
static int pinctrl_lib_control_get_function_groups(struct device *dev,
						   u16_t func, u16_t *groups,
						   u16_t *num_groups)
{
	const struct pinctrl_lib_config *config =
		pinctrl_lib_device_config_info(dev);

	if (func >= config->function_count) {
		*num_groups = 0;
		return -ENODEV;
	}

	u16_t num_groups_found = 0;

	for (u16_t i = 0; i < config->pinctrl_state_count; i++) {
		if (func == config->pinctrl_state_data[i].function) {
			if (num_groups_found < *num_groups) {
				groups[num_groups_found] = i;
			}
			num_groups_found++;
		}
	}
	if (num_groups_found == 0) {
		*num_groups = 0;
		return -ENOTSUP;
	}
	if (num_groups_found > *num_groups) {
		*num_groups = num_groups_found;
		return -EINVAL;
	}
	*num_groups = num_groups_found;
	return 0;
}

/**
 * Get the state of given name that can be applied to given function.
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param func Function the state shall be applied to.
 * @param name State name
 * @param[out] state State
 * @retval 0 on success
 * @retval -ENOTSUP if there is no state with the requested name for
 *                  the function
 * @retval -ENODEV if requested function is not available on this controller
 */
static int pinctrl_lib_control_get_function_state(struct device *dev,
						  u16_t func,
						  const char *name,
						  u16_t *state)
{
	const struct pinctrl_lib_config *config =
		pinctrl_lib_device_config_info(dev);

	if (func >= config->function_count) {
		return -ENODEV;
	}

	for (u16_t i = 0; i < config->pinctrl_state_count; i++) {
		u16_t function = config->pinctrl_state_data[i].function;
		const char *state_name =
			config->state_name_data[config->pinctrl_state_data[i]
						.name].name;

		if ((func == function) && (strcmp(name, state_name) == 0)) {
			*state = i;
			return 0;
		}
	}

	return -ENOTSUP;
}

/**
 * Get the states that can be applied to the function.
 *
 * Returns an array of state numbers. The state number can be used with
 * pinctrl_get_state_group() to retrieve the associated pin group.
 * The applicable states are returned in @p states and the number of states
 * in @p num_states. The number of states returned is bounded by the initial
 * value of @p num_states when called.
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param func Function
 * @param[out] states Array of states.
 * @param[in,out] num_states Size of the array of states.
 * @retval 0 on success
 * @retval -ENODEV if function is unknown to pin controller
 * @retval -ENOTSUP if there is no state
 * @retval -EINVAL if the number of available states exceeds array size
 */
static int pinctrl_lib_control_get_function_states(struct device *dev,
						   u16_t func, u16_t *states,
						   u16_t *num_states)
{
	const struct pinctrl_lib_config *config =
		pinctrl_lib_device_config_info(dev);

	if (func >= config->function_count) {
		*num_states = 0;
		return -ENODEV;
	}

	u16_t num_states_found = 0;

	for (u16_t i = 0; i < config->pinctrl_state_count; i++) {
		if (func == config->pinctrl_state_data[i].function) {
			if (num_states_found < *num_states) {
				states[num_states_found] = i;
			}
			num_states_found++;
		}
	}

	if (num_states_found == 0) {
		*num_states = 0;
		return -ENOTSUP;
	}
	if (num_states_found > *num_states) {
		*num_states = num_states_found;
		return -EINVAL;
	}
	*num_states = num_states_found;
	return 0;
}

/**
 * Get the function that is associated to a client device.
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param client Pointer to the device structure of the client driver instance.
 * @param[out] func Function associated to the client device.
 * @retval 0 on success
 * @retval -ENOTSUP if requested function is not available on this controller
 */
static int pinctrl_lib_control_get_device_function(struct device *dev,
						   struct device *client,
						   u16_t *func)
{
	const struct pinctrl_lib_config *config =
		pinctrl_lib_device_config_info(dev);

	for (u32_t i = 0; i < config->function_count; i++) {
		if (strcmp(client->config->name,
			   config->function_data[i].name) == 0) {
			*func = i;
			return 0;
		}
	}

	return -ENOTSUP;
}

/**
 * Get the pin controller pin number and pin mapping for a GPIO pin.
 *
 * @note Pin controller pins are numbered consecutively.
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param gpio Pointer to the device structure of the GPIO driver instance.
 * @param gpio_pin GPIO pin in GPIO number space.
 * @param[out] pin GPIO pin in pin-controller number space.
 * @param[out] base_pin Base pin of gpio range in pin-controller number space.
 * @param[out] num_pins Number of pins in gpio range.
 * @retval 0 on success
 * @retval -ENOTSUP if there is no gpio range for the requested GPIO pin.
 */
static int pinctrl_lib_control_get_gpio_range(struct device *dev,
					      struct device *gpio,
					      u32_t gpio_pin, u16_t *pin,
					      u16_t *base_pin,
					      u8_t *num_pins)
{
	const struct pinctrl_lib_config *config =
		pinctrl_lib_device_config_info(dev);

	int gpio_pin_idx = gpio_port_pin_idx(gpio_pin);

	if (gpio_pin_idx < 0) {
		/* invalid gpio_pin */
		return gpio_pin_idx;
	}
	for (u8_t i = 0; i < config->gpio_range_count; i++) {
		if ((strcmp(gpio->config->name,
			    config->gpio_range_data[i].name) == 0) &&
		    (gpio_pin_idx >=
		     config->gpio_range_data[i].gpio_base_idx) &&
		    (gpio_pin_idx < config->gpio_range_data[i].npins)) {
			*pin = config->gpio_range_data[i].base +
				(gpio_pin_idx -
				 config->gpio_range_data[i].gpio_base_idx);
			*base_pin = config->gpio_range_data[i].base;
			*num_pins = config->gpio_range_data[i].npins;
			return 0;
		}
	}

	return -ENOTSUP;
}

/* Wrapper to access externally provided function */
static inline int pinctrl_lib_config_get(struct device *dev, u16_t pin,
					 u32_t *config)
{
	return CONFIG_PINCTRL_LIB_CONFIG_GET(dev, pin, config);
}

/* Wrapper to access externally provided function */
static inline int pinctrl_lib_config_set(struct device *dev, u16_t pin,
					 u32_t config)
{
	return CONFIG_PINCTRL_LIB_CONFIG_SET(dev, pin, config);
}

/**
 * @brief Get the configuration of a pin group.
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param group Pin group
 * @param[out] configs set to the configuration of the pins in the group
 * @param[in,out] num_configs number of pin configurations requested/ set.
 * @retval 0 on success
 * @retval -ENOTSUP if configuration read out is not available for this group
 * @retval -EINVAL if the number of available configurations exceeds requested
 */
static int pinctrl_lib_config_group_get(struct device *dev, u16_t group,
					u32_t *configs, u16_t *num_configs)
{
	int err;
	const struct pinctrl_lib_config *config =
		pinctrl_lib_device_config_info(dev);

	if (group >= config->pinctrl_state_count) {
		*num_configs = 0;
		return -ENOTSUP;
	}

	u16_t num_pins_found = 0;

	for (int i = 0; i < config->pinctrl_count; i++) {
		if (config->pinctrl_data[i].state == group) {
			if (num_pins_found < *num_configs) {
				err = pinctrl_lib_config_get(dev, i,
							     &configs
							     [num_pins_found]);
				if (err) {
					*num_configs = num_pins_found;
					return err;
				}
			}
			num_pins_found++;
		}
	}
	if (num_pins_found == 0) {
		*num_configs = 0;
		return -ENOTSUP;
	}
	if (num_pins_found > *num_configs) {
		*num_configs = num_pins_found;
		return -EINVAL;
	}
	*num_configs = num_pins_found;
	return 0;
}

/**
 * @brief Configure a group of pins.
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param group Pin group
 * @param configs The configurations of the pins in the group
 * @param num_configs number of pin configurations to set.
 * @retval 0 on success
 * @retval -ENOTSUP if configuration is not available for this group
 * @retval -EINVAL if number of configurations does not match pins in group
 */
static int pinctrl_lib_config_group_set(struct device *dev, u16_t group,
					const u32_t *configs,
					u16_t num_configs)
{
	int err;
	const struct pinctrl_lib_config *config =
		pinctrl_lib_device_config_info(dev);

	if (group >= config->pinctrl_state_count) {
		return -ENOTSUP;
	}

	u16_t num_pins_found = 0;

	for (int i = 0; i < config->pinctrl_count; i++) {
		if (config->pinctrl_data[i].state == group) {
			if (num_pins_found < num_configs) {
				err = pinctrl_lib_config_set(dev, i,
							     configs
							     [num_pins_found]);
				if (err) {
					return err;
				}
			}
			num_pins_found++;
		}
	}
	if (num_pins_found == 0) {
		return -ENOTSUP;
	}
	if (num_pins_found != num_configs) {
		return -EINVAL;
	}
	return 0;
}

#if CONFIG_PINCTRL_LIB_ENABLE_MUX_REQUEST

/**
 * @brief Request a pin for muxing.
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param pin Pin
 * @param owner A representation of the owner; typically the device
 *		name that controls its mux function, or the requested GPIO name
 * @retval 0 on success
 * @retval -EBUSY pin already used
 * @retval -ENOTSUP requested pin is not available on this controller
 * @retval -ENOMEM can not handle request
 */
static int pinctrl_lib_mux_request(struct device *dev, u16_t pin,
				   const char *owner)
{
	const char *current_owner;

	if (pin >= pinctrl_lib_device_config_info(dev)->pin_count) {
		return -ENOTSUP;
	}
	current_owner = pinctrl_lib_mux_owner_get(dev, pin);
	if (current_owner == 0) {
		return pinctrl_lib_mux_owner_set(dev, pin, owner);
	}
	if (current_owner != owner) {
		return -EBUSY;
	}
	return 0;
}

/**
 * @brief Release a pin for muxing.
 *
 * After release another may become owner of the pin.
 *
 * Only the owner may release a muxed pin.
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param pin Pin
 * @param owner A representation of the owner; typically the device
 *		name that controls its mux function, or the requested GPIO name
 * @retval 0 on success
 * @retval -EACCES owner does not own the pin
 * @retval -ENOTSUP requested pin is not available on this controller
 */
static int pinctrl_lib_mux_free(struct device *dev, u16_t pin,
				const char *owner)
{
	const char *current_owner;

	if (pin >= pinctrl_lib_device_config_info(dev)->pin_count) {
		return -ENOTSUP;
	}
	current_owner = pinctrl_lib_mux_owner_get(dev, pin);
	if (current_owner != owner) {
		return -EACCES;
	}
	return pinctrl_lib_mux_owner_set(dev, pin, 0);
}
#endif /* CONFIG_PINCTRL_LIB_ENABLE_MUX_REQUEST */

/* Wrapper to access externally provided function */
static inline int pinctrl_lib_mux_get(struct device *dev, u16_t pin,
				      u16_t *func)
{
	return PINCTRL_LIB_MUX_GET(dev, pin, func);
}

/* Wrapper to access externally provided function */
static inline int pinctrl_lib_mux_set(struct device *dev, u16_t pin,
				      u16_t func)
{
	return PINCTRL_LIB_MUX_SET(dev, pin, func);
}

/**
 * @brief Set muxing function for a group of pins
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param group Group
 * @param func Muxing function
 * @retval 0 on success
 * @retval -ENOTSUP if requested function or group is not available
 *                  on this controller
 * @retval -EINVAL group does no belong to function
 */
static int pinctrl_lib_mux_group_set(struct device *dev, u16_t group,
				     u16_t func)
{
	const struct pinctrl_lib_config *config =
		pinctrl_lib_device_config_info(dev);
	int err;

	if (func >= config->function_count) {
		/* function does not denote a device */
		return -ENOTSUP;
	}
	if (group >= config->pinctrl_state_count) {
		/* pin group does not correspond to a pinctrl state */
		return -ENOTSUP;
	}
	if (config->pinctrl_state_data[group].function != func) {
		/* Group (aka pinctrl state) was defined by a different
		 * function (aka. device)
		 */
		return -EINVAL;
	}
	u16_t num_pins_found = 0;

	for (u16_t i = 0; i < config->pinctrl_count; i++) {
		if (config->pinctrl_data[i].state == group) {
			err = pinctrl_lib_mux_set(dev,
						  config->pinctrl_data[i].pin,
						  config->pinctrl_data[i].mux);
			if (err) {
				return err;
			}
			num_pins_found++;
		}
	}
	if (num_pins_found == 0) {
		/* no pins found to mux pin group to function */
		return -ENOTSUP;
	}
	return 0;
}

/**
 * @brief Set pinctrl state.
 *
 * @param dev Pointer to the device structure for the driver instance.
 * @param state State (index into pinctrl state data)
 * @retval 0 on success
 * @retval -ENOTSUP if requested state is not available on this controller
 */
static int pinctrl_lib_state_set(struct device *dev, u16_t state)
{
	const struct pinctrl_lib_config *config =
		pinctrl_lib_device_config_info(dev);
	int err = -ENOTSUP; /* Returned if no corresponding state is found */

	if (state >= config->pinctrl_state_count) {
		return err;
	}

	for (int i = 0; i < config->pinctrl_count; i++) {
		if (config->pinctrl_data[i].state == state) {
#if CONFIG_PINCTRL_LIB_ENABLE_MUX_REQUEST
			const char *owner =
				config->function_data[
					config->pinctrl_state_data[
						state].function].name;

			err = pinctrl_lib_mux_request(
				dev, config->pinctrl_data[i].pin, owner);
			if (err) {
				return err;
			}
#endif
			err = pinctrl_lib_config_set(dev,
				config->pinctrl_data[i].pin,
				config->pinctrl_data[i].config);
			if (err) {
				return err;
			}
			err = pinctrl_lib_mux_set(dev,
				config->pinctrl_data[i].pin,
				config->pinctrl_data[i].mux);
			if (err) {
				return err;
			}
		}
	}
	return err;
}

/**
 * @brief Initialise pin controller.
 *
 * @param dev Pointer to the device structure for the driver instance.
 */
static int pinctrl_lib_init(struct device *dev)
{
	const struct pinctrl_lib_config *config =
		pinctrl_lib_device_config_info(dev);
	int err;

	err = config->device_init(dev);
	if (err) {
		return err;
	}
#if CONFIG_PINCTRL_LIB_ENABLE_MUX_REQUEST
	err = pinctrl_lib_mux_request_init(dev);
	if (err) {
		return err;
	}
#endif
	for (u16_t state = 0; (state < config->pinctrl_state_count) && !err;
	     state++) {
		const char *state_name =
			config->state_name_data
			[config->pinctrl_state_data[state].name]
			.name;

		if (strcmp(state_name, "default") == 0) {
			err = pinctrl_lib_state_set(dev, state);
		}
	}
	return err;
}

static const struct pinctrl_driver_api pinctrl_lib_driver_api = {
	.control = {
		    .get_pins_count = pinctrl_lib_control_get_pins_count,
		    .get_groups_count = pinctrl_lib_control_get_groups_count,
		    .get_group_pins = pinctrl_lib_control_get_group_pins,
		    .get_states_count = pinctrl_lib_control_get_states_count,
		    .get_state_group = pinctrl_lib_control_get_state_group,
		    .get_functions_count =
		    pinctrl_lib_control_get_functions_count,
		    .get_function_group =
		    pinctrl_lib_control_get_function_group,
		    .get_function_groups =
		    pinctrl_lib_control_get_function_groups,
		    .get_function_state =
		    pinctrl_lib_control_get_function_state,
		    .get_function_states =
		    pinctrl_lib_control_get_function_states,
		    .get_device_function =
		    pinctrl_lib_control_get_device_function,
		    .get_gpio_range = pinctrl_lib_control_get_gpio_range,
		    },
	.config = {
		   .get = PINCTRL_LIB_CONFIG_GET,
		   .set = PINCTRL_LIB_CONFIG_SET,
		   .group_get = pinctrl_lib_config_group_get,
		   .group_set = pinctrl_lib_config_group_set,
		   },
	.mux = {
#if CONFIG_PINCTRL_LIB_ENABLE_MUX_REQUEST
		.request = pinctrl_lib_mux_request,
		.free = pinctrl_lib_mux_free,
#endif
		.get = PINCTRL_LIB_MUX_GET,
		.set = PINCTRL_LIB_MUX_SET,
		.group_set = pinctrl_lib_mux_group_set,
		},
	.state = {
		  .set = pinctrl_lib_state_set,
		  },
};

/**
 * @def PINCTRL_LIB_CONTROLLER_CONFIG
 * @brief Helper for configuration info initialization.
 *
 * Creates an initialization for configuration info for controller
 * with index idx.
 *
 * To be used by CHAOS_PP_REPEAT_FROM_TO the signature must be of macro(s, n).
 *
 * @param idx Controller index [0..3]
 */
#define PINCTRL_LIB_CONTROLLER_CONFIG(s, idx) \
	_PINCTRL_LIB_CONTROLLER_CONFIG1(      \
		idx,                          \
		PINCTRL_LIB_CONTROLLER_CONFIG_INFO(idx))
#define _PINCTRL_LIB_CONTROLLER_CONFIG1(idx, _config_info)        \
	_PINCTRL_LIB_CONTROLLER_CONFIG2(                          \
		_config_info,                                     \
		CHAOS_PP_CAT(_config_info, _mux_pinmap),          \
		PINCTRL_LIB_CONTROLLER_PIN_COUNT(idx),            \
		CHAOS_PP_CAT(_config_info, _function),            \
		PINCTRL_LIB_CONTROLLER_FUNCTION_COUNT(idx),       \
		PINCTRL_LIB_CONTROLLER_REPEAT_FUNCTION(idx),      \
		CHAOS_PP_CAT(_config_info, _state_name),          \
		PINCTRL_LIB_CONTROLLER_STATE_NAME_COUNT(idx),     \
		PINCTRL_LIB_CONTROLLER_REPEAT_STATE_NAME(idx),    \
		CHAOS_PP_CAT(_config_info, _pinctrl_state),       \
		PINCTRL_LIB_CONTROLLER_PINCTRL_STATE_COUNT(idx),  \
		PINCTRL_LIB_CONTROLLER_REPEAT_PINCTRL_STATE(idx), \
		CHAOS_PP_CAT(_config_info, _pinctrl),             \
		PINCTRL_LIB_CONTROLLER_PINCTRL_COUNT(idx),        \
		PINCTRL_LIB_CONTROLLER_REPEAT_PINCTRL(idx),       \
		CHAOS_PP_CAT(_config_info, _gpio_range),          \
		PINCTRL_LIB_CONTROLLER_GPIO_RANGE_COUNT(idx),     \
		PINCTRL_LIB_CONTROLLER_REPEAT_GPIO_RANGE(idx),    \
		PINCTRL_LIB_CONTROLLER_DEVICE_INIT(idx))
#define _PINCTRL_LIB_CONTROLLER_CONFIG2(_config_info,                          \
					_mux_pinmap,                           \
					_pin_count,                            \
					_function_data,                        \
					_function_count,                       \
					_repeat_function,                      \
					_state_name_data,                      \
					_state_name_count,                     \
					_repeat_state_name,                    \
					_pinctrl_state_data,                   \
					_pinctrl_state_count,                  \
					_repeat_pinctrl_state,                 \
					_pinctrl_data,                         \
					_pinctrl_count,                        \
					_repeat_pinctrl,                       \
					_gpio_range_data,                      \
					_gpio_range_count,                     \
					_repeat_gpio_range,                    \
					_device_init)                          \
	CHAOS_PP_WHEN(CONFIG_PINCTRL_LIB_ENABLE_MUX_REQUEST)(                  \
	static u8_t _mux_pinmap[(_pin_count + 1) >>  1];)                      \
	static const struct pinctrl_lib_function                               \
		_function_data[_function_count] = {                            \
			CHAOS_PP_EXPR(CHAOS_PP_REPEAT_FROM_TO(                 \
				0, _function_count, _repeat_function))};       \
	static const struct pinctrl_lib_state_name                             \
		_state_name_data[_state_name_count] = {                        \
			CHAOS_PP_EXPR(CHAOS_PP_REPEAT_FROM_TO(                 \
				0, _state_name_count, _repeat_state_name))};   \
	static const struct pinctrl_lib_pinctrl_state                          \
		_pinctrl_state_data[_pinctrl_state_count] = {                  \
			CHAOS_PP_EXPR(CHAOS_PP_REPEAT_FROM_TO(                 \
				0, _pinctrl_state_count,                       \
				_repeat_pinctrl_state))};                      \
	static const struct pinctrl_lib_pinctrl                                \
		_pinctrl_data[_pinctrl_count] = {                              \
			CHAOS_PP_EXPR(CHAOS_PP_REPEAT_FROM_TO(                 \
				0, _pinctrl_count, _repeat_pinctrl))};         \
	static const struct pinctrl_lib_gpio_range                             \
		_gpio_range_data[_gpio_range_count] = {                        \
			CHAOS_PP_EXPR(CHAOS_PP_REPEAT_FROM_TO(                 \
				0, _gpio_range_count, _repeat_gpio_range))};   \
	static const struct pinctrl_lib_config _config_info = {                \
		.pin_count = _pin_count,                                       \
		.function_count = _function_count,                             \
		.state_name_count = _state_name_count,                         \
		.pinctrl_state_count = _pinctrl_state_count,                   \
		.pinctrl_count = _pinctrl_count,                               \
		.gpio_range_count = _gpio_range_count,                         \
		.function_data = _function_data,                               \
		.state_name_data = _state_name_data,                           \
		.pinctrl_state_data = _pinctrl_state_data,                     \
		.pinctrl_data = _pinctrl_data,                                 \
		.gpio_range_data = _gpio_range_data,                           \
		CHAOS_PP_WHEN(CONFIG_PINCTRL_LIB_ENABLE_MUX_REQUEST)(          \
		.mux_pinmap = _mux_pinmap,)                                    \
		.device_init = _device_init,                                   \
	};                                                                     \
	DRIVER_SETUP_END()

/**
 * @def PINCTRL_LIB_DEVICE
 * @brief Helper for device info initialization.
 *
 * Creates an initialization for device info for controller with index idx.
 *
 * To be used by CHAOS_PP_REPEAT_FROM_TO the signature must be of macro(s, n).
 *
 * @param idx Controller index [0..3]
 */
#define PINCTRL_LIB_DEVICE(s, idx)                                             \
	_PINCTRL_LIB_DEVICE1(idx,                                              \
			     PINCTRL_LIB_CONTROLLER_DEVICE_NAME(idx),          \
			     PINCTRL_LIB_CONTROLLER_DRIVER_NAME(idx),          \
			     pinctrl_lib_init,                                 \
			     PINCTRL_LIB_CONTROLLER_DATA(idx),                 \
			     PINCTRL_LIB_CONTROLLER_CONFIG_INFO(idx),          \
			     POST_KERNEL,                                      \
			     PINCTRL_LIB_CONTROLLER_DEVICE_INIT_PRIORITY(idx), \
			     pinctrl_lib_driver_api)
#define _PINCTRL_LIB_DEVICE1(idx,          \
			     _dev_name,    \
			     _drv_name,    \
			     _init_fn,     \
			     _data,        \
			     _config_info, \
			     _level,       \
			     _prio,        \
			     _api)         \
	_PINCTRL_LIB_DEVICE2(idx,          \
			     _dev_name,    \
			     _drv_name,    \
			     _init_fn,     \
			     _data,        \
			     _config_info, \
			     _level,       \
			     _prio,        \
			     _api)
#define _PINCTRL_LIB_DEVICE2(idx,          \
			     _dev_name,    \
			     _drv_name,    \
			     _init_fn,     \
			     _data,        \
			     _config_info, \
			     _level,       \
			     _prio,        \
			     _api)         \
	DEVICE_AND_API_INIT(_dev_name,     \
			    _drv_name,     \
			    _init_fn,      \
			    _data,         \
			    &_config_info, \
			    _level,        \
			    _prio,         \
			    &_api);

/* Setup configuration data for all PINCTRL devices */
CHAOS_PP_EXPR(CHAOS_PP_REPEAT_FROM_TO(0, PINCTRL_LIB_CONTROLLER_COUNT,
				      PINCTRL_LIB_CONTROLLER_CONFIG))

/* Setup all PINCTRL devices */
CHAOS_PP_EXPR(CHAOS_PP_REPEAT_FROM_TO(0, PINCTRL_LIB_CONTROLLER_COUNT,
				      PINCTRL_LIB_DEVICE))

/** @} pinctrl_library */
#endif /* __PINCTRL_LIB_H */
