/*
 * Copyright (c) 2018 Bobby Noelte
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _DRIVER_LIB_H_
#define _DRIVER_LIB_H_

/* Assure this info is available at the earliest time.
 * It may be needed for DRIVER_COMPATIBLE at the beginning
 * of a file before it is included via other include files.
 */
#include <autoconf.h>
#include <generated_dts_board.h>
#include <soc.h>

#include <chaos/preprocessor/cat.h>
#include <chaos/preprocessor/comparison/equal.h>
#include <chaos/preprocessor/comparison/less.h>
#include <chaos/preprocessor/control/iif.h>
#include <chaos/preprocessor/detection/is_empty.h>
#include <chaos/preprocessor/facilities/empty.h>
#include <chaos/preprocessor/facilities/expand.h>
#include <chaos/preprocessor/repetition/repeat_from_to.h>

/**
 * @brief Device Driver Library
 * @defgroup device_driver_lib Device Driver Library
 * @ingroup device_model
 * @{
 */

/**
 * @def DRIVER_CONTROLLER_COUNT
 * @brief Get the number of activated devices of given type.
 *
 * @attention The SOC_[type]_CONTROLLER_COUNT macro shall be defined in
 *            generated_dts_board.h. This is achieved by a [type]-controller
 *            directive in the device tree.
 *
 * @param type Type of device (e.g. SPI, GPIO, PINCTRL, ...)
 * @return number of activated devices
 */
#define DRIVER_CONTROLLER_COUNT(type)      \
	CHAOS_PP_EXPAND(CHAOS_PP_CAT(SOC_, \
			CHAOS_PP_PRIMITIVE_CAT(type, _CONTROLLER_COUNT)))

/**
 * @def DRIVER_DTS_PREFIX
 * @brief Get the device tree prefix for the device of the given type and idx.
 *
 * @attention The SOC_[type]_CONTROLLER_[idx] macro shall be defined in
 *            generated_dts_board.h. This is achieved by a [type]-controller
 *            directive in the device tree.
 *
 * @param type Type of device (e.g. SPI, GPIO, PINCTRL, ...)
 * @param idx Index of device
 * @return device tree prefix (e.g. ST_STM32_SPI_FIFO_4000000)
 */
#define DRIVER_DTS_PREFIX(type, idx)                                 \
	CHAOS_PP_EXPAND(CHAOS_PP_CAT(                                \
			CHAOS_PP_CAT(SOC_,                           \
			CHAOS_PP_PRIMITIVE_CAT(type, _CONTROLLER_)), \
			CHAOS_PP_EXPAND(idx)))

/**
 * @def DRIVER_ACTIVATED
 * @brief Check whether a device of given type is activated in the device tree.
 *
 * This is a convenience macro for:
 *   - DRIVER_CONTROLLER_COUNT(type)
 *
 * @attention The SOC_[type]_CONTROLLER_COUNT macro shall be defined in
 *            generated_dts_board.h. This is achieved by a [type]-controller
 *            directive in the device tree.
 *
 * @param type Type of device (e.g. SPI, GPIO, PINCTRL, ...)
 * @return > 0 if at least one device is activated, 0 otherwise
 */
#define DRIVER_ACTIVATED(type) \
	DRIVER_CONTROLLER_COUNT(type)

/**
 * @def DRIVER_COMPATIBLE
 * @brief Check compatibility to at least one activated device of given type.
 *
 * The @p compatible parameter is checked against the compatible property of
 * all activated devices of given @p type.
 *
 * The macro may be used as an #if guard to exclude the driver source
 * from compilation - even if the driver is configured by Kconfig.
 *
 * @note The @p compatible parameter shall be sanitized to the label form.
 *       (e.g. st,stm32-spi-fifo shall become ST_STM32_SPI_FIFO).
 *
 * @attention Only up to 4 compatible labels are checked.
 *            Only up to 16 devices of given type are checked.
 *
 * @param type Type of device (e.g. SPI, GPIO, PINCTRL, ...)
 * @param compatible driver compatibility string sanitized for label usage
 *                   (e.g. ST_STM32_SPI_FIFO)
 * @return 1 if there is compatibility to at least one activated device,
 *         0 otherwise
 */
#define DRIVER_COMPATIBLE(type, compatible)                           \
	CHAOS_PP_EXPAND(                                              \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_IF(0, type, compatible)) (1,  \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_IF(1, type, compatible)) (1,  \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_IF(2, type, compatible)) (1,  \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_IF(3, type, compatible)) (1,  \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_IF(4, type, compatible)) (1,  \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_IF(5, type, compatible)) (1,  \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_IF(6, type, compatible)) (1,  \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_IF(7, type, compatible)) (1,  \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_IF(8, type, compatible)) (1,  \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_IF(9, type, compatible)) (1,  \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_IF(10, type, compatible)) (1, \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_IF(11, type, compatible)) (1, \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_IF(12, type, compatible)) (1, \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_IF(13, type, compatible)) (1, \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_IF(14, type, compatible)) (1, \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_IF(15, type, compatible)) (1, \
	0)))))))))))))))))
#define _DRIVER_COMPATIBLE_IF(x, type, compatible)                      \
	CHAOS_PP_EXPAND(                                                \
	CHAOS_PP_IIF(CHAOS_PP_LESS(x, DRIVER_CONTROLLER_COUNT(type))) ( \
		DRIVER_COMPATIBLE_X(type, x, compatible), 0))

/**
 * @def DRIVER_COMPATIBLE_X
 * @brief Check compatibility to device given by type and idx.
 *
 * The @p compatible parameter is checked against the compatible property of
 * the device given by @p type and @p idx.
 *
 * @note The @p compatible parameter shall be sanitized to the label form.
 *       (e.g. st,stm32-spi-fifo shall be ST_STM32_SPI_FIFO).
 *
 * @attention Only up to 4 compatible labels are checked.
 *
 * @param type Type of device (e.g. SPI, GPIO, PINCTRL, ...)
 * @param idx Index of device
 * @param compatible driver compatibility string sanitized for label usage
 *                   (e.g. ST_STM32_SPI_FIFO)
 * @return 1 if compatible, 0 otherwise
 */
#define DRIVER_COMPATIBLE_X(type, idx, compatible)                          \
	CHAOS_PP_EXPAND(                                                    \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_X_IF(0, type, idx, compatible)) (1, \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_X_IF(1, type, idx, compatible)) (1, \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_X_IF(2, type, idx, compatible)) (1, \
	CHAOS_PP_IIF(_DRIVER_COMPATIBLE_X_IF(3, type, idx, compatible)) (1, \
	0)))))
#define _DRIVER_COMPATIBLE_X_IF(x, type, idx, compatible)           \
	CHAOS_PP_EXPAND(                                            \
	CHAOS_PP_IIF(CHAOS_PP_LESS(                                 \
		x, DRIVER_PROPERTY(type, idx, COMPATIBLE_COUNT))) ( \
		_DRIVER_COMPATIBLE_X(x, type, idx, compatible), 0))
#define _DRIVER_COMPATIBLE_X(x, type, idx, compatible)        \
	CHAOS_PP_EQUAL(                                       \
		DRIVER_PROPERTY(                              \
			type,                                 \
			idx,                                  \
			CHAOS_PP_CAT(                         \
			CHAOS_PP_CAT(                         \
			CHAOS_PP_CAT(COMPATIBLE_, x), _ID))), \
		CHAOS_PP_EXPAND(                              \
			CHAOS_PP_CAT(                         \
			SOC_COMPATIBLE_ID_,                   \
			CHAOS_PP_EXPAND(compatible)))         \
	)

/**
 * @def DRIVER_PROPERTY
 * @brief Get driver property value.
 *
 * Get the property value of the property defined in generated_dts_board.h.
 *
 * @attention The SOC_[type]_CONTROLLER_[idx] macro shall be defined in
 *            generated_dts_board.h. This is achieved by a [type]-controller
 *            directive in the device tree.
 *
 * @param type Type of device (e.g. SPI, GPIO, PINCTRL, ...)
 * @param idx Index of device
 * @param property Property name of the device tree property define
 *                 (e.g. BASE_ADDRESS, LABEL, IRQ_0, ...)
 * @return property value
 */
#define DRIVER_PROPERTY(type, idx, property)              \
	CHAOS_PP_EXPAND(                                  \
		CHAOS_PP_CAT(                             \
		CHAOS_PP_CAT(                             \
			DRIVER_DTS_PREFIX(type, idx), _), \
			CHAOS_PP_EXPAND(property)))

/**
 * @def DRIVER_PROPERTY_INDIRECT
 * @brief Get the property of another device given by a property.
 *
 * @param type Type of device (e.g. SPI, GPIO, PINCTRL, ...)
 * @param idx Index of device
 * @param property Property that denotes the dts prefix of the other device
 * @param property_indirect property of the other device
 * @return property value of the other device
 */
#define DRIVER_PROPERTY_INDIRECT(type, idx, property, property_indirect) \
	CHAOS_PP_EXPAND(                                                 \
		CHAOS_PP_CAT(                                            \
		CHAOS_PP_CAT(                                            \
			DRIVER_PROPERTY(type, idx, property), _),        \
			CHAOS_PP_EXPAND(property_indirect)))

/**
 * @def DRIVER_NAME
 * @brief Get the driver name.
 *
 * This is a convenience macro for:
 *   - DRIVER_PROPERTY(type, idx, LABEL)
 *
 * @attention The SOC_[type]_CONTROLLER_[idx] macro shall be defined in
 *            generated_dts_board.h. This is achieved by a [type]-controller
 *            directive in the device tree.
 *
 * @param type Type of device (e.g. SPI, GPIO, PINCTRL, ...)
 * @param idx Index of device
 * @return driver name (e.g. "SPI_0")
 */
#define DRIVER_NAME(type, idx) \
	DRIVER_PROPERTY(type, idx, LABEL)

/**
 * @def DRIVER_NAME_INDIRECT
 * @brief Get the driver name of another device given by a property.
 *
 * This is a convenience macro for:
 *   - DRIVER_PROPERTY_INDIRECT(type, idx, property, LABEL)
 *
 * @attention The SOC_[type]_CONTROLLER_[idx] macro shall be defined in
 *            generated_dts_board.h. This is achieved by a [type]-controller
 *            directive in the device tree.
 *
 * @param type Type of device (e.g. SPI, GPIO, PINCTRL, ...)
 * @param idx Index of device
 * @param property Property that denotes the dts prefix of the other device
 * @return driver name of other device (e.g. "PINCTRL")
 */
#define DRIVER_NAME_INDIRECT(type, idx, property) \
	DRIVER_PROPERTY_INDIRECT(type, idx, property, LABEL)

/**
 * @def DRIVER_DEVICE_NAME
 * @brief Get the device name.
 *
 * The device tree prefix is used as the device name.
 *
 * @attention The SOC_[type]_CONTROLLER_[idx] macro shall be defined in
 *            generated_dts_board.h. This is achieved by a [type]-controller
 *            directive in the device tree.
 *
 * @param type Type of device (e.g. SPI, GPIO, PINCTRL, ...)
 * @param idx Index of device
 * @return device name (e.g. ST_STM32_SPI_FIFO_4000000)
 */
#define DRIVER_DEVICE_NAME(type, idx) \
	DRIVER_DTS_PREFIX(type, idx)

/**
 * @def DRIVER_DATA
 * @brief Get the name of the driver data.
 *
 * The macro generates an unique name for driver data.
 *
 * @attention The SOC_[type]_CONTROLLER_[idx] macro shall be defined in
 *            generated_dts_board.h. This is achieved by a [type]-controller
 *            directive in the device tree.
 *
 * @param type Type of device (e.g. SPI, GPIO, PINCTRL, ...)
 * @param idx Index of device
 * @param data suffix for data
 * @return driver data name (e.g. ST_STM32_SPI_FIFO_4000000_config)
 */
#define DRIVER_DATA(type, idx, data)              \
	CHAOS_PP_CAT(                             \
	CHAOS_PP_CAT(                             \
		DRIVER_DTS_PREFIX(type, idx), _), \
		CHAOS_PP_EXPAND(data))

/**
 * @def DRIVER_SETUP
 * @brief Repeat the setup macro for all activated devices given by type.
 *
 * The signature of the setup macro must be of setup(s, idx).
 * idx denotes the device index, s is the macro state internally used.
 *
 * @attention The SOC_[type]_CONTROLLER_COUNT and the
 *            SOC_[type]_CONTROLLER_[idx] macros shall be defined in
 *            generated_dts_board.h. This is achieved by a [type]-controller
 *            directive in the device tree.
 *
 * @param type Type of device (e.g. SPI, GPIO, PINCTRL, ...)
 * @param setup Macro to setup the device with given idx.
 */
#define DRIVER_SETUP(type, setup)                                          \
	CHAOS_PP_EXPR(CHAOS_PP_REPEAT_FROM_TO(                             \
		0,                                                         \
		CHAOS_PP_EXPAND(CHAOS_PP_CAT(SOC_,                         \
			CHAOS_PP_PRIMITIVE_CAT(type, _CONTROLLER_COUNT))), \
		setup))

/**
 * @def DRIVER_SETUP_END
 * @brief Macro to follow a trailing semicolon at the end of a setup macro.
 *
 * The macro expands to nothing. Its only purpose is to make checkpatch
 * happy with an intended trailing semicolon. Can not always fool checkpatch.
 *
 * @code{.c}
 * #define my_setup(s, idx)       \
 *         my_data = {0, 1, 2};   \
 *         your_data = {0, 1, 2}; \
 *         DRIVER_SETUP_END()
 * @endcode
 *
 */
#define DRIVER_SETUP_END CHAOS_PP_EMPTY

/* @} device_driver_lib */

#endif	/* _DRIVER_LIB_H_ */
