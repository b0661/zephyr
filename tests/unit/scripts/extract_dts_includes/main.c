/*
 * Copyright (c) 2018 Bobby Noelte
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>
#include <zephyr/types.h>
#include <gpio_common.h>
#include <generated/generated_dts_board.h>

#include "pinctrl_test.h"

static _Bool assert_pinmux(u32_t a_port, u32_t a_mux, u32_t b_port, u32_t b_mux)
{
	if (a_port != b_port) {
		return 0;
	}
	if (a_mux != b_mux) {
		return 0;
	}
	return 1;
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

void test_pinctrl_define_directives(void)
{
	/* assure define directives are generated */
	zassert(assert_pinmux(TEST_DEVICE_4000C000_PINCTRL_DEFAULT_RX_PINMUX,
			      1000,
			      2000),
		"pass",
		"fail");
	zassert(TEST_DEVICE_4000C000_PINCTRL_DEFAULT_RX_BIAS_PULL_UP == 1000,
		"pass",
		"fail");
	zassert(TEST_DEVICE_4000C000_PINCTRL_DEFAULT_RX_INPUT_ENABLE,
		"pass",
		"fail");
	zassert(assert_pinmux(TEST_DEVICE_4000C000_PINCTRL_DEFAULT_TX_PINMUX,
			      1001,
			      2001),
		"pass",
		"fail");
	zassert(TEST_DEVICE_4000C000_PINCTRL_DEFAULT_TX_BIAS_PULL_DOWN == 1001,
		"pass",
		"fail");
	zassert(TEST_DEVICE_4000C000_PINCTRL_DEFAULT_TX_OUTPUT_ENABLE,
		"pass",
		"fail");
	/* assure default pin control define directives are generated */
	zassert(assert_pinmux(
			TEST_PINCTRL_40000000_PINCTRL_0_PINMUX, 1000, 2000),
		"pass",
		"fail");
	zassert(TEST_PINCTRL_40000000_PINCTRL_0_BIAS_PULL_UP == 1000,
		"pass",
		"fail");
	zassert(TEST_PINCTRL_40000000_PINCTRL_0_INPUT_ENABLE, "pass", "fail");
	zassert(assert_pinmux(
			TEST_PINCTRL_40000000_PINCTRL_1_PINMUX, 1001, 2001),
		"pass",
		"fail");
	zassert(TEST_PINCTRL_40000000_PINCTRL_1_BIAS_PULL_DOWN == 1001,
		"pass",
		"fail");
	zassert(TEST_PINCTRL_40000000_PINCTRL_1_OUTPUT_ENABLE, "pass", "fail");

/* assure non-linux pinctrl-binding directives are created */
#define xstr2(s) str2(s)
#define xstr(s) str(s)
#define str2(s1, s2) (#s1 "," #s2)
#define str(s) #s
#define TEST_PINMUX_LEGACY0 \
	xstr2(TEST_DEVICE_6000C000_PINMUX_DEVICE2_0_RX_TX_FUNCTION_0)
#define TEST_PINMUX_LEGACY1 \
	xstr2(TEST_DEVICE_6000C000_PINMUX_DEVICE2_0_RX_TX_FUNCTION_1)
#define TEST_PINMUX_LEGACY2 \
	xstr(TEST_DEVICE_6000C000_PINMUX_DEVICE2_0_RX_TX_PIN_0)
#define TEST_PINMUX_LEGACY3 \
	xstr(TEST_DEVICE_6000C000_PINMUX_DEVICE2_0_RX_TX_PIN_1)

	zassert(assert_strcmp(TEST_PINMUX_LEGACY0, "[1234,5678]"),
		"pass",
		"fail");
	zassert(assert_strcmp(TEST_PINMUX_LEGACY1, "[2345,6789]"),
		"pass",
		"fail");
	zassert(assert_strcmp(TEST_PINMUX_LEGACY2, "rx"), "pass", "fail");
	zassert(assert_strcmp(TEST_PINMUX_LEGACY3, "tx"), "pass", "fail");
}

void test_pinctrl_default_init(void)
{
#define TEST_PINCTRL_DEFAULT_INIT(label, idx)                               \
	_TEST_PINCTRL_DEFAULT_INIT1(label##_PINCTRL_##idx##_PINMUX,         \
				    label##_PINCTRL_##idx##_BIAS_PULL_UP,   \
				    label##_PINCTRL_##idx##_BIAS_PULL_DOWN, \
				    label##_PINCTRL_##idx##_INPUT_ENABLE,   \
				    label##_PINCTRL_##idx##_OUTPUT_ENABLE)
#define _TEST_PINCTRL_DEFAULT_INIT1(_mux, _up, _down, _in, _out) \
	_TEST_PINCTRL_DEFAULT_INIT2(_mux, _up, _down, _in, _out)
#define _TEST_PINCTRL_DEFAULT_INIT2(_mux0, _mux1, _up, _down, _in, _out) \
	{.pinmux = {_mux0, _mux1},                                       \
	 .bias_pull_up = _up,                                            \
	 .bias_pull_down = _down,                                        \
	 .input_enable = _in,                                            \
	 .output_enable = _out},

	struct s_pinctrl_init {
		u32_t pinmux[2];
		u32_t bias_pull_up;
		u32_t bias_pull_down;
		_Bool input_enable;
		_Bool output_enable;
	};

	const struct s_pinctrl_init pinctrl_init[] = {
#if TEST_PINCTRL_40000000_PINCTRL_COUNT > 0
		TEST_PINCTRL_DEFAULT_INIT(TEST_PINCTRL_40000000, 0)
#endif
#if TEST_PINCTRL_40000000_PINCTRL_COUNT > 1
		TEST_PINCTRL_DEFAULT_INIT(TEST_PINCTRL_40000000, 1)
#endif
#if TEST_PINCTRL_40000000_PINCTRL_COUNT > 2
		TEST_PINCTRL_DEFAULT_INIT(TEST_PINCTRL_40000000, 2)
#endif
#if TEST_PINCTRL_40000000_PINCTRL_COUNT > 3
		TEST_PINCTRL_DEFAULT_INIT(TEST_PINCTRL_40000000, 3)
#endif

/* unused default configuration index 4 */
#if TEST_PINCTRL_40000000_PINCTRL_COUNT > 4
		TEST_PINCTRL_DEFAULT_INIT(TEST_PINCTRL_40000000, 4)
#endif
	};

	/* http://www.geeksforgeeks.org/how-to-find-size-of-array-in-cc/
	 * -without-using-sizeof-operator/
	 */
	const int pinctrl_init_size = *(&pinctrl_init + 1) - pinctrl_init;

	zassert_equal(TEST_PINCTRL_40000000_PINCTRL_COUNT,
		      4,
		      "pin control init size");
	zassert_equal(pinctrl_init_size,
		      TEST_PINCTRL_40000000_PINCTRL_COUNT,
		      "pin control init size");
	zassert_equal(pinctrl_init[0].bias_pull_up,
		      TEST_PINCTRL_40000000_PINCTRL_0_BIAS_PULL_UP,
		      "");
	zassert_equal(pinctrl_init[0].bias_pull_down,
		      TEST_PINCTRL_40000000_PINCTRL_0_BIAS_PULL_DOWN,
		      "");
	zassert_equal(pinctrl_init[0].input_enable,
		      TEST_PINCTRL_40000000_PINCTRL_0_INPUT_ENABLE,
		      "");
	zassert_equal(pinctrl_init[0].output_enable,
		      TEST_PINCTRL_40000000_PINCTRL_0_OUTPUT_ENABLE,
		      "");
	zassert_equal(pinctrl_init[1].bias_pull_up,
		      TEST_PINCTRL_40000000_PINCTRL_1_BIAS_PULL_UP,
		      "");
	zassert_equal(pinctrl_init[1].bias_pull_down,
		      TEST_PINCTRL_40000000_PINCTRL_1_BIAS_PULL_DOWN,
		      "");
	zassert_equal(pinctrl_init[1].input_enable,
		      TEST_PINCTRL_40000000_PINCTRL_1_INPUT_ENABLE,
		      "");
	zassert_equal(pinctrl_init[1].output_enable,
		      TEST_PINCTRL_40000000_PINCTRL_1_OUTPUT_ENABLE,
		      "");
}

void test_pinctrl_pins_default_init(void)
{
#define PIN_A 23
#define PIN_B 34

#define TEST_PINCTRL_PINS_INIT(label, idx)                            \
	_TEST_PINCTRL_PINS_INIT1(label##_PINCTRL_##idx##_OUTPUT_HIGH, \
				 label##_PINCTRL_##idx##_PINS)
#define _TEST_PINCTRL_PINS_INIT1(_high, _pins) \
	_TEST_PINCTRL_PINS_INIT2(_high, _pins, 0, 0, 0, 0, 0, 0)
#define _TEST_PINCTRL_PINS_INIT2(                             \
	_high, _pin0, _pin1, _pin2, _pin3, _pin4, _pin5, ...) \
	{.pins = {_pin0, _pin1, _pin2, _pin3, _pin4, _pin5},  \
	 .output_high = _high},

	struct s_pinctrl_init {
		u32_t pins[6];
		_Bool output_high;
	};

	const struct s_pinctrl_init pinctrl_init[] = {
#if TEST_PINCTRL_50000000_PINCTRL_COUNT > 0
		TEST_PINCTRL_PINS_INIT(TEST_PINCTRL_50000000, 0)
#endif

/* unused default configuration index 001 */
#if TEST_PINCTRL_50000000_PINCTRL_COUNT > 1
		TEST_PINCTRL_PINS_INIT(TEST_PINCTRL_50000000, 1)
#endif
#if TEST_PINCTRL_50000000_PINCTRL_COUNT > 2
		TEST_PINCTRL_PINS_INIT(TEST_PINCTRL_50000000, 2)
#endif
	};

	const int pinctrl_init_size = *(&pinctrl_init + 1) - pinctrl_init;

	zassert_equal(TEST_PINCTRL_50000000_PINCTRL_COUNT,
		      1,
		      "pin control init size");
	zassert_equal(pinctrl_init_size,
		      TEST_PINCTRL_50000000_PINCTRL_COUNT,
		      "pin control init size");
	zassert_equal(pinctrl_init[0].pins[0], PIN_A, "");
	zassert_equal(pinctrl_init[0].pins[1], PIN_B, "");
	zassert_equal(pinctrl_init[0].pins[2], 0, "");
	zassert_equal(pinctrl_init[0].output_high,
		      TEST_PINCTRL_50000000_PINCTRL_0_OUTPUT_HIGH,
		      "");
}

void test_gpio_ranges(void)
{
	/* defines are generated */
	zassert_equal(TEST_GPIO_10000000_GPIO_RANGE_0_BASE,
		      GPIO_PORT_PIN0,
		      "gpio-ranges gpio port pin base");
	zassert_equal(TEST_GPIO_10000000_GPIO_RANGE_0_NPINS,
		      16,
		      "gpio-ranges gpio port pins number");
	zassert_equal(TEST_GPIO_10000000_GPIO_RANGE_0_CONTROLLER_BASE,
		      16,
		      "gpio-ranges controller port pin base");
	zassert(assert_strcmp(xstr(TEST_GPIO_10000000_GPIO_RANGE_0_CONTROLLER),
			      "TEST_PINCTRL_40000000"),
		"pass",
		"fail");
}

void test_device_controller(void)
{
	/* defines are generated */
	zassert_equal(SOC_PIN_CONTROLLER_COUNT,
		      2,
		      "soc pin controller count: %d",
		      (int)SOC_PIN_CONTROLLER_COUNT);
	zassert_equal(SOC_GPIO_CONTROLLER_COUNT,
		      3,
		      "soc gpio controller count: %d",
		      (int)SOC_GPIO_CONTROLLER_COUNT);
}

void test_main(void)
{
	ztest_test_suite(test_extract_dts_includes,
			 ztest_unit_test(test_pinctrl_define_directives),
			 ztest_unit_test(test_pinctrl_default_init),
			 ztest_unit_test(test_pinctrl_pins_default_init),
			 ztest_unit_test(test_gpio_ranges),
			 ztest_unit_test(test_device_controller));
	ztest_run_test_suite(test_extract_dts_includes);
}
