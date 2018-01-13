/*
 * Copyright (c) 2018 Bobby Noelte
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>

#include "../../../../drivers/driver_lib.h"
#include <zephyr/types.h>
#include <device.h>

/* Generic test defines */
#define TEST_PIN_COUNT 264

/* Defines for values from Device Tree */
#define TEST_DRIVER_NAME ST_STM32_PINCTRL_48000000_LABEL
#define TEST_FUNCTION_COUNT ST_STM32_PINCTRL_48000000_FUNCTION_COUNT
#define TEST_PINCTRL_STATE_COUNT ST_STM32_PINCTRL_48000000_PINCTRL_STATE_COUNT
#define TEST_PINCTRL_COUNT ST_STM32_PINCTRL_48000000_PINCTRL_COUNT
#define TEST_CLIENT_NAME ST_STM32_USART_40004400_LABEL
#define TEST_CLIENT_FUNCTION \
	ST_STM32_USART_40004400_PINCTRL_CONTROLLER_0_FUNCTION_ID
#define TEST_CLIENT_DEFAULT_STATE \
	ST_STM32_USART_40004400_PINCTRL_DEFAULT_STATE_ID
#define TEST_CLIENT_GROUP TEST_CLIENT_DEFAULT_STATE
#define TEST_CLIENT_GROUP_PINS 2 /* rx, tx */

/* Test mock */
struct mock_info {
	int config_get_invocation;
	int config_set_invocation;
	int mux_get_invocation;
	int mux_set_invocation;
	u32_t mux_set_pin;
	u16_t mux_set_func;
	int device_init_invocation;
	struct device_config client_config;
	struct device client;
};

struct mock_info mock_data;

#define MOCK_PINCONF_PINMUX_MUX(pin, mux) mux
#define MOCK_PINCONF_PINMUX_PIN(pin, mux) pin

/* forward declarations */
static int mock_config_get(struct device *dev, u16_t pin, u32_t *config)
{
	mock_data.config_get_invocation++;
	return 0;
}

static int mock_config_set(struct device *dev, u16_t pin, u32_t config)
{
	mock_data.config_set_invocation++;
	return 0;
}

static int mock_mux_get(struct device *dev, u16_t pin, u16_t *func)
{
	mock_data.mux_get_invocation++;
	return 0;
}

static int mock_mux_set(struct device *dev, u16_t pin, u16_t func)
{
	mock_data.mux_set_invocation++;
	mock_data.mux_set_pin = pin;
	mock_data.mux_set_func = func;
	return 0;
}

static int mock_device_init(struct device *dev)
{
	mock_data.device_init_invocation++;
	return 0;
}

/* Configure PINCTRL library */
#define CONFIG_PINCTRL_LIB_SELECT_PINMUX 1
#define CONFIG_PINCTRL_LIB_ENABLE_MUX_REQUEST 1
#define CONFIG_PINCTRL_LIB_DEVICE_NAME_PREFIX pinctrl_test
#define CONFIG_PINCTRL_LIB_CONFIG_GET mock_config_get
#define CONFIG_PINCTRL_LIB_CONFIG_SET mock_config_set
#define CONFIG_PINCTRL_LIB_MUX_GET mock_mux_get
#define CONFIG_PINCTRL_LIB_MUX_SET mock_mux_set
#define CONFIG_PINCTRL_LIB_PINCONF_PINMUX_PIN MOCK_PINCONF_PINMUX_PIN
#define CONFIG_PINCTRL_LIB_PINCONF_PINMUX_MUX MOCK_PINCONF_PINMUX_MUX
#define CONFIG_PINCTRL_LIB_CONTROLLER_COUNT 1
#define CONFIG_PINCTRL_LIB_CONTROLLER_0_PREFIX ST_STM32_PINCTRL_48000000
#define CONFIG_PINCTRL_LIB_CONTROLLER_0_DATA (&mock_data)
#define CONFIG_PINCTRL_LIB_CONTROLLER_0_PIN_COUNT TEST_PIN_COUNT
#define CONFIG_PINCTRL_LIB_CONTROLLER_0_DEVICE_INIT mock_device_init

#include <drivers/pinctrl/pinctrl_lib.h>

/* Defines for what DEVICE_API_INIT generates */
#define TEST_DEVICE _TEST_DEVICE1(PINCTRL_LIB_CONTROLLER_DEVICE_NAME(0))
#define _TEST_DEVICE1(_device_name) _TEST_DEVICE2(_device_name)
#define _TEST_DEVICE2(_device_name) DEVICE_NAME_GET(_device_name)

/* Access pinctrl Lib data structures */
#define TEST_API pinctrl_lib_driver_api
#define TEST_DEVICE_CONFIG_INFO(_device_name) \
	_TEST_DEVICE_CONFIG_INFO1(_device_name)
#define _TEST_DEVICE_CONFIG_INFO1(_device_name) \
	((const struct pinctrl_lib_config *)_device_name.config->config_info)

/* Replace Pinctrl API syscall interface */
__syscall u16_t pinctrl_get_pins_count(struct device *dev)
{
	return _impl_pinctrl_get_pins_count(dev);
}

__syscall u16_t pinctrl_get_groups_count(struct device *dev)
{
	return _impl_pinctrl_get_groups_count(dev);
}

__syscall int pinctrl_get_group_pins(struct device *dev, u16_t group,
				     u16_t *pins, u16_t *num_pins)
{
	return _impl_pinctrl_get_group_pins(dev, group, pins, num_pins);
}

__syscall u16_t pinctrl_get_states_count(struct device *dev)
{
	return _impl_pinctrl_get_states_count(dev);
}

__syscall int pinctrl_get_state_group(struct device *dev, u16_t state,
				      u16_t *group)
{
	return _impl_pinctrl_get_state_group(dev, state, group);
}

__syscall u16_t pinctrl_get_functions_count(struct device *dev)
{
	return _impl_pinctrl_get_functions_count(dev);
}

__syscall int pinctrl_get_function_group(struct device *dev, u16_t func,
					 const char *name, u16_t *group)
{
	return _impl_pinctrl_get_function_group(dev, func, name, group);
}

__syscall int pinctrl_get_function_groups(struct device *dev, u16_t func,
					  u16_t *groups, u16_t *num_groups)
{
	return _impl_pinctrl_get_function_groups(dev, func, groups, num_groups);
}

__syscall int pinctrl_get_function_state(struct device *dev, u16_t func,
					 const char *name, u16_t *state)
{
	return _impl_pinctrl_get_function_state(dev, func, name, state);
}

__syscall int pinctrl_get_function_states(struct device *dev, u16_t func,
					  u16_t *states, u16_t *num_states)
{
	return _impl_pinctrl_get_function_states(dev, func, states, num_states);
}

__syscall int pinctrl_get_device_function(struct device *dev,
					  struct device *other, u16_t *func)
{
	return _impl_pinctrl_get_device_function(dev, other, func);
}

__syscall int pinctrl_get_gpio_range(struct device *dev, struct device *gpio,
				     u32_t gpio_pin, u16_t *pin,
				     u16_t *base_pin, u8_t *num_pins)
{
	return _impl_pinctrl_get_gpio_range(
		dev, gpio, gpio_pin, pin, base_pin, num_pins);
}

__syscall int pinctrl_config_get(struct device *dev, u16_t pin, u32_t *config)
{
	return _impl_pinctrl_config_get(dev, pin, config);
}

__syscall int pinctrl_config_set(struct device *dev, u16_t pin, u32_t config)
{
	return _impl_pinctrl_config_set(dev, pin, config);
}

__syscall int pinctrl_config_group_get(struct device *dev, u16_t group,
				       u32_t *configs, u16_t *num_configs)
{
	return _impl_pinctrl_config_group_get(dev, group, configs, num_configs);
}

__syscall int pinctrl_config_group_set(struct device *dev, u16_t group,
				       const u32_t *configs, u16_t num_configs)
{
	return _impl_pinctrl_config_group_set(dev, group, configs, num_configs);
}

__syscall int pinctrl_mux_request(struct device *dev, u16_t pin,
				  const char *owner)
{
	return _impl_pinctrl_mux_request(dev, pin, owner);
}

__syscall int pinctrl_mux_free(struct device *dev, u16_t pin, const char *owner)
{
	return _impl_pinctrl_mux_free(dev, pin, owner);
}

__syscall int pinctrl_mux_get(struct device *dev, u16_t pin, u16_t *func)
{
	return _impl_pinctrl_mux_get(dev, pin, func);
}

__syscall int pinctrl_mux_set(struct device *dev, u16_t pin, u16_t func)
{
	return _impl_pinctrl_mux_set(dev, pin, func);
}

__syscall int pinctrl_mux_group_set(struct device *dev, u16_t group, u16_t func)
{
	return _impl_pinctrl_mux_group_set(dev, group, func);
}

__syscall int pinctrl_state_set(struct device *dev, u16_t state)
{
	return _impl_pinctrl_state_set(dev, state);
}

static _Bool assert_strcmp(const char *s1, const char *s2)
{
	const char *_s1 = s1;
	const char *_s2 = s2;

	for (int i = 0; i <= 1000; i++) {
		if ((*s1 == 0) && (*s2 == 0)) {
			return 1;
		}
		if (*s1 != *s2) {
			TC_PRINT("%s: %s, %s failed - char %c != %c (%d)\n",
				 __func__, _s1, _s2, *s1, *s2, i);
			return 0;
		}
		if (((*s1 == 0) && (*s2 != 0)) || ((*s1 != 0) && (*s2 == 0))) {
			TC_PRINT("%s: %s, %s failed - end of string (%d)\n",
				 __func__, _s1, _s2, i);
			return 0;
		}
		if (i >= 1000) {
			TC_PRINT("%s: %s, %s failed - no end of string (%d)\n",
				 __func__, _s1, _s2, i);
			return 0;
		}
		s1++;
		s2++;
	};
	return 1;
}

static void mock_reset(void)
{
	mock_data.config_get_invocation = 0;
	mock_data.config_set_invocation = 0;
	mock_data.mux_get_invocation = 0;
	mock_data.mux_set_invocation = 0;
	mock_data.device_init_invocation = 0;
	mock_data.client_config.name = TEST_CLIENT_NAME;
	mock_data.client_config.init = 0;
	mock_data.client_config.config_info = 0;
	mock_data.client.config = &mock_data.client_config,
	mock_data.client.driver_api = 0;
	mock_data.client.driver_data = 0;

	/* fake init of mux_request data */
	pinctrl_lib_mux_owner_init();
	zassert_equal(0,
		      pinctrl_lib_mux_request_init(&TEST_DEVICE),
		      "init: pinctrl_lib_mux_request_init failed");
}

void test_pinctrl_lib_init(void)
{
	zassert_equal(TEST_API.config.get,
		      mock_config_get,
		      "init: pinctrl_config_get API init failed");
	zassert_equal(TEST_API.config.set,
		      mock_config_set,
		      "init: pinctrl_config_get API init failed");
	zassert_equal(TEST_API.mux.get,
		      mock_mux_get,
		      "init: pinctrl_mux_get API init failed");
	zassert_equal(TEST_API.mux.set,
		      mock_mux_set,
		      "init: pinctrl_mux_set API init failed");
	zassert_equal(TEST_API.state.set,
		      pinctrl_lib_state_set,
		      "init: pinctrl_state_set API init failed");

	zassert_equal(1,
		      assert_strcmp(TEST_DRIVER_NAME, TEST_DEVICE.config->name),
		      "init: driver name init failed");
	zassert_equal(TEST_FUNCTION_COUNT,
		      TEST_DEVICE_CONFIG_INFO(TEST_DEVICE)->function_count,
		      "init: function_count init failed");
	zassert_equal(mock_device_init,
		      TEST_DEVICE_CONFIG_INFO(TEST_DEVICE)->device_init,
		      "init: device_init init failed");

	/* call device init function */
	mock_reset();
	zassert_equal(0,
		      TEST_DEVICE.config->init(&TEST_DEVICE),
		      "init: init() failed");
	zassert_equal(1,
		      mock_data.device_init_invocation,
		      "init: device_init() not called");
	zassert_equal(TEST_PINCTRL_COUNT,
		      mock_data.config_set_invocation,
		      "init: default initialisation (config) not called");
	zassert_equal(TEST_PINCTRL_COUNT,
		      mock_data.mux_set_invocation,
		      "init: default initialisation (mux) not called");
}

void test_pinctrl_lib_control_get_pins_count(void)
{
	mock_reset();

	zassert_equal(CONFIG_PINCTRL_LIB_CONTROLLER_0_PIN_COUNT,
		      pinctrl_get_pins_count(&TEST_DEVICE),
		      "api: pinctrl_get_pins_count failed");
}

void test_pinctrl_lib_control_get_function_state(void)
{
	u16_t state = 0;

	mock_reset();
	zassert_equal(
		0,
		pinctrl_get_function_state(
			&TEST_DEVICE, TEST_CLIENT_FUNCTION, "default", &state),
		"api: pinctrl_get_function_state failed");
	zassert_equal(TEST_CLIENT_DEFAULT_STATE,
		      state,
		      "api: pinctrl_get_function_state wrong state");
}

void test_pinctrl_lib_control_get_function_states(void)
{
	u16_t states[TEST_PINCTRL_STATE_COUNT];
	u16_t num_states;

	mock_reset();
	/* Only default state configured */
	num_states = TEST_PINCTRL_STATE_COUNT;
	states[0] = 0;
	states[1] = 0;
	zassert_equal(0,
		      pinctrl_get_function_states(&TEST_DEVICE,
						  TEST_CLIENT_FUNCTION,
						  &states[0],
						  &num_states),
		      "api: pinctrl_get_function_states failed");
	zassert_equal(1,
		      num_states,
		      "api: pinctrl_get_function_states wrong state count");
	zassert_equal(TEST_CLIENT_DEFAULT_STATE,
		      states[0],
		      "api: pinctrl_get_function_state wrong state");
	/* error - not sufficent array space */
	num_states = 0;
	states[0] = 0;
	states[1] = 0;
	zassert_equal(-EINVAL,
		      pinctrl_get_function_states(&TEST_DEVICE,
						  TEST_CLIENT_FUNCTION,
						  &states[0],
						  &num_states),
		      "api: pinctrl_get_function_states failed");
	zassert_equal(1,
		      num_states,
		      "api: pinctrl_get_function_states wrong state count");
	zassert_equal(
		0, states[0], "api: pinctrl_get_function_state wrong state");
	/* error - unknown function */
	num_states = 2;
	states[0] = 0;
	states[1] = 0;
	zassert_equal(-ENODEV,
		      pinctrl_get_function_states(&TEST_DEVICE,
						  TEST_FUNCTION_COUNT,
						  &states[0],
						  &num_states),
		      "api: pinctrl_get_function_states failed");
	zassert_equal(0,
		      num_states,
		      "api: pinctrl_get_function_states wrong state count");
	zassert_equal(
		0, states[0], "api: pinctrl_get_function_state wrong state");
}

void test_pinctrl_lib_control_get_device_function(void)
{
	u16_t func = 0;

	mock_reset();
	zassert_equal(0,
		      pinctrl_get_device_function(
			      &TEST_DEVICE, &mock_data.client, &func),
		      "api: pinctrl_get_device_function failed");
	zassert_equal(TEST_CLIENT_FUNCTION,
		      func,
		      "api: pinctrl_get_device_function wrong function");
}

void test_pinctrl_lib_config_group_get(void)
{
	u16_t group;
	u16_t num_configs;
	u32_t configs[TEST_PIN_COUNT];

	/* normal case */
	mock_reset();
	group = TEST_CLIENT_GROUP;
	num_configs = TEST_PIN_COUNT;
	zassert_equal(0,
		      pinctrl_lib_config_group_get(
			      &TEST_DEVICE, group, &configs[0], &num_configs),
		      "api: pinctrl_lib_config_group_get failed");
	zassert_equal(
		TEST_CLIENT_GROUP_PINS,
		num_configs,
		"api: pinctrl_lib_config_group_get wrong config count: %d",
		(int)num_configs);
	zassert_equal(
		TEST_CLIENT_GROUP_PINS,
		mock_data.config_get_invocation,
		"api: pinctrl_lib_config_group_get config_get called: %d times",
		(int)mock_data.config_get_invocation);
	/* error - configs array (aka. num_configs) to small */
	mock_reset();
	group = TEST_CLIENT_GROUP;
	num_configs = TEST_CLIENT_GROUP_PINS - 1;
	zassert_equal(-EINVAL,
		      pinctrl_lib_config_group_get(
			      &TEST_DEVICE, group, &configs[0], &num_configs),
		      "api: pinctrl_lib_config_group_get failed");
	zassert_equal(
		TEST_CLIENT_GROUP_PINS,
		num_configs,
		"api: pinctrl_lib_config_group_get wrong config count: %d",
		(int)num_configs);
	zassert_equal(
		TEST_CLIENT_GROUP_PINS - 1,
		mock_data.config_get_invocation,
		"api: pinctrl_lib_config_group_get config_get called: %d times",
		(int)mock_data.config_get_invocation);
	/* error - unknown group */
	mock_reset();
	group = TEST_PINCTRL_STATE_COUNT;
	num_configs = TEST_PIN_COUNT;
	zassert_equal(-ENOTSUP,
		      pinctrl_lib_config_group_get(
			      &TEST_DEVICE, group, &configs[0], &num_configs),
		      "api: pinctrl_lib_config_group_get failed");
	zassert_equal(
		0,
		num_configs,
		"api: pinctrl_lib_config_group_get wrong config count: %d",
		(int)num_configs);
	zassert_equal(
		0,
		mock_data.config_get_invocation,
		"api: pinctrl_lib_config_group_get config_get called: %d times",
		(int)mock_data.config_get_invocation);
}

void test_pinctrl_lib_config_group_set(void)
{
	u16_t group;
	u16_t num_configs;
	u32_t configs[TEST_PIN_COUNT];

	/* normal case */
	mock_reset();
	group = TEST_CLIENT_GROUP;
	num_configs = TEST_CLIENT_GROUP_PINS;
	zassert_equal(0,
		      pinctrl_lib_config_group_set(
			      &TEST_DEVICE, group, &configs[0], num_configs),
		      "api: pinctrl_lib_config_group_set failed");
	zassert_equal(
		TEST_CLIENT_GROUP_PINS,
		mock_data.config_set_invocation,
		"api: pinctrl_lib_config_group_set config_set called: %d times",
		(int)mock_data.config_set_invocation);
	/* error - configs array (aka. num_configs) to small */
	mock_reset();
	group = TEST_CLIENT_GROUP;
	num_configs = TEST_CLIENT_GROUP_PINS - 1;
	zassert_equal(-EINVAL,
		      pinctrl_lib_config_group_set(
			      &TEST_DEVICE, group, &configs[0], num_configs),
		      "api: pinctrl_lib_config_group_set failed");
	zassert_equal(
		TEST_CLIENT_GROUP_PINS - 1,
		mock_data.config_set_invocation,
		"api: pinctrl_lib_config_group_set config_set called: %d times",
		(int)mock_data.config_set_invocation);
	/* error - unknown group */
	mock_reset();
	group = TEST_PINCTRL_STATE_COUNT;
	num_configs = TEST_PIN_COUNT;
	zassert_equal(-ENOTSUP,
		      pinctrl_lib_config_group_set(
			      &TEST_DEVICE, group, &configs[0], num_configs),
		      "api: pinctrl_lib_config_group_set failed");
	zassert_equal(
		0,
		mock_data.config_set_invocation,
		"api: pinctrl_lib_config_group_set config_set called: %d times",
		(int)mock_data.config_set_invocation);
}

void test_pinctrl_lib_mux_request_free(void)
{
	const char *owner1 = "xxxx";
	const char *owner2 = "yyyy";

	mock_reset();

	/* owner1 requests all pins - all pins should be available */
	for (u16_t pin = 0; pin < CONFIG_PINCTRL_LIB_CONTROLLER_0_PIN_COUNT;
	     pin++) {
		zassert_equal(0,
			      pinctrl_mux_request(&TEST_DEVICE, pin, owner1),
			      "api: pinctrl_mux_request failed (pin: %d)",
			      (int)pin);
	}
	/* owner2 requests all pins - none should be available */
	for (u16_t pin = 0; pin < CONFIG_PINCTRL_LIB_CONTROLLER_0_PIN_COUNT;
	     pin++) {
		zassert_equal(
			-EBUSY,
			pinctrl_mux_request(&TEST_DEVICE, pin, owner2),
			"api: pinctrl_mux_request wrongly passed (pin: %d)",
			(int)pin);
	}
	/* owner1 frees all pins - all pins should be available afterwards */
	for (u16_t pin = 0; pin < CONFIG_PINCTRL_LIB_CONTROLLER_0_PIN_COUNT;
	     pin++) {
		zassert_equal(0,
			      pinctrl_mux_free(&TEST_DEVICE, pin, owner1),
			      "api: pinctrl_mux_free failed (pin: %d)",
			      (int)pin);
	}
	/* owner2 requests all pins - all pins should be available */
	for (u16_t pin = 0; pin < CONFIG_PINCTRL_LIB_CONTROLLER_0_PIN_COUNT;
	     pin++) {
		zassert_equal(0,
			      pinctrl_mux_request(&TEST_DEVICE, pin, owner2),
			      "api: pinctrl_mux_request failed (pin: %d)",
			      (int)pin);
	}
	/* owner1 tries to free all pins - should not be possible - not owner */
	for (u16_t pin = 0; pin < CONFIG_PINCTRL_LIB_CONTROLLER_0_PIN_COUNT;
	     pin++) {
		zassert_equal(-EACCES,
			      pinctrl_mux_free(&TEST_DEVICE, pin, owner1),
			      "api: pinctrl_mux_free wrongly passed (pin: %d)",
			      (int)pin);
	}
	/* owner2 frees all pins - all pins should be available afterwards */
	for (u16_t pin = 0; pin < CONFIG_PINCTRL_LIB_CONTROLLER_0_PIN_COUNT;
	     pin++) {
		zassert_equal(0,
			      pinctrl_mux_free(&TEST_DEVICE, pin, owner2),
			      "api: pinctrl_mux_free failed (pin: %d)",
			      (int)pin);
	}
	/* owner1 requests all pins - all pins should be available */
	for (u16_t pin = 0; pin < CONFIG_PINCTRL_LIB_CONTROLLER_0_PIN_COUNT;
	     pin++) {
		zassert_equal(0,
			      pinctrl_mux_request(&TEST_DEVICE, pin, owner1),
			      "api: pinctrl_mux_request failed (pin: %d)",
			      (int)pin);
	}
	zassert_equal(
		-ENOTSUP,
		pinctrl_mux_request(&TEST_DEVICE,
				    CONFIG_PINCTRL_LIB_CONTROLLER_0_PIN_COUNT,
				    owner2),
		"api: pinctrl_mux_request wrongly passed (pin: %d)",
		(int)CONFIG_PINCTRL_LIB_CONTROLLER_0_PIN_COUNT);
}

void test_pinctrl_lib_mux_group_set(void)
{
	u16_t group;
	u16_t func;

	/* normal case */
	mock_reset();
	group = TEST_CLIENT_GROUP;
	func = TEST_CLIENT_FUNCTION;
	zassert_equal(0,
		      pinctrl_lib_mux_group_set(&TEST_DEVICE, group, func),
		      "api: pinctrl_lib_mux_group_set failed");
	zassert_equal(
		TEST_CLIENT_GROUP_PINS,
		mock_data.mux_set_invocation,
		"api: pinctrl_lib_mux_group_set config_set called: %d times",
		(int)mock_data.mux_set_invocation);
	/* error - unknown function */
	mock_reset();
	group = TEST_CLIENT_GROUP;
	func = TEST_FUNCTION_COUNT;
	zassert_equal(-ENOTSUP,
		      pinctrl_lib_mux_group_set(&TEST_DEVICE, group, func),
		      "api: pinctrl_lib_mux_group_set failed");
	zassert_equal(
		0,
		mock_data.mux_set_invocation,
		"api: pinctrl_lib_mux_group_set config_set called: %d times",
		(int)mock_data.mux_set_invocation);
	/* error - unknown group */
	mock_reset();
	group = TEST_PINCTRL_STATE_COUNT;
	func = TEST_CLIENT_FUNCTION;
	zassert_equal(-ENOTSUP,
		      pinctrl_lib_mux_group_set(&TEST_DEVICE, group, func),
		      "api: pinctrl_lib_mux_group_set failed");
	zassert_equal(
		0,
		mock_data.mux_set_invocation,
		"api: pinctrl_lib_mux_group_set config_set called: %d times",
		(int)mock_data.mux_set_invocation);
}

void test_pinctrl_lib_state_set(void)
{
	mock_reset();
	zassert_equal(
		0,
		pinctrl_state_set(&TEST_DEVICE, TEST_CLIENT_DEFAULT_STATE),
		"api: pinctrl_state_set failed");
}

void test_pinctrl_pinmux(void)
{
	mock_reset();
	zassert_equal(0,
		      pinmux_pin_set(&TEST_DEVICE, 1, 2),
		      "pinctrl_pinmux: pinmux_pin_set() failed");
	zassert_equal(1,
		      mock_data.mux_set_invocation,
		      "pinctrl_pinmux: mux_set not called");
}

void test_main(void)
{
	ztest_test_suite(
		test_pinctrl_lib,
		ztest_unit_test(test_pinctrl_lib_init),
		ztest_unit_test(test_pinctrl_lib_control_get_pins_count),
		ztest_unit_test(test_pinctrl_lib_control_get_function_state),
		ztest_unit_test(test_pinctrl_lib_control_get_function_states),
		ztest_unit_test(test_pinctrl_lib_control_get_device_function),
		ztest_unit_test(test_pinctrl_lib_config_group_get),
		ztest_unit_test(test_pinctrl_lib_config_group_set),
		ztest_unit_test(test_pinctrl_lib_mux_request_free),
		ztest_unit_test(test_pinctrl_lib_mux_group_set),
		ztest_unit_test(test_pinctrl_lib_state_set),
		ztest_unit_test(test_pinctrl_pinmux));
	ztest_run_test_suite(test_pinctrl_lib);
}
