/*
 * Copyright (c) b0661n0e17e@gmail.com
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ztest.h>
#include <zephyr/types.h>
#include "outdir/generated_dts.h"

static _Bool assert_pinmux(u32_t a_port, u32_t a_mux,
                           u32_t b_port, u32_t b_mux)
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
        for(int i = 0; i <= 1000; i++) {
                if ((*s1 == 0) && (*s2 == 0)) {
                        return 1;
                }
                if (*s1 != *s2) {
                        TC_PRINT("assert_strcmp: %s, %s failed - "\
                                "char %c != %c (%d)\n", _s1, _s2, *s1, *s2, i);
                        return 0;
                }
                if (((*s1 == 0) && (*s2 != 0)) || ((*s1 != 0) && (*s2 == 0))) {
                        TC_PRINT("assert_strcmp: %s, %s failed - "\
                                "end of string (%d)\n", _s1, _s2, i);
                        return 0;
                }
                if (i >= 1000) {
                        TC_PRINT("assert_strcmp: %s, %s failed - "\
                                "no end of string (%d)\n", _s1, _s2, i);
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
	zassert(assert_pinmux(
                TEST_DEVICE_4000C000_PINCTRL_RX_DEFAULT_PINMUX,1234,5678),
                "pass", "fail");
	zassert(TEST_DEVICE_4000C000_PINCTRL_RX_DEFAULT_BIAS_PULL_UP \
                == 50000, "pass", "fail");
	zassert(TEST_DEVICE_4000C000_PINCTRL_RX_DEFAULT_INPUT_ENABLE,
                "pass", "fail");
	zassert(assert_pinmux(
                TEST_DEVICE_4000C000_PINCTRL_TX_DEFAULT_PINMUX,2345,6789),
                "pass", "fail");
	zassert(TEST_DEVICE_4000C000_PINCTRL_TX_DEFAULT_BIAS_PULL_DOWN \
                == 100000, "pass", "fail");
	zassert(TEST_DEVICE_4000C000_PINCTRL_TX_DEFAULT_OUTPUT_ENABLE,
                "pass", "fail");
        /* assure default pin control define directives are generated */
	zassert(assert_pinmux(
                TEST_PINMUX_40000000_PINCTRL_000_PINMUX,1234,5678),
                "pass", "fail");
	zassert(TEST_PINMUX_40000000_PINCTRL_000_BIAS_PULL_UP == 50000,
                "pass", "fail");
	zassert(TEST_PINMUX_40000000_PINCTRL_000_INPUT_ENABLE,
                "pass", "fail");
	zassert(assert_pinmux(
                TEST_PINMUX_40000000_PINCTRL_001_PINMUX,2345,6789),
                "pass", "fail");
	zassert(TEST_PINMUX_40000000_PINCTRL_001_BIAS_PULL_DOWN == 100000,
                "pass", "fail");
	zassert(TEST_PINMUX_40000000_PINCTRL_001_OUTPUT_ENABLE,
                "pass", "fail");
        /* assure non-linux pinctrl-binding directives are created */
#define xstr2(s) str2(s)
#define xstr(s) str(s)
#define str2(s1,s2) #s1 "," #s2
#define str(s) #s
#define TEST_PINMUX_LEGACY0 \
        xstr2(TEST_DEVICE_6000C000_PINMUX_DEVICE2_0_RX_TX_FUNCTION_0)
#define TEST_PINMUX_LEGACY1 \
        xstr2(TEST_DEVICE_6000C000_PINMUX_DEVICE2_0_RX_TX_FUNCTION_1)
#define TEST_PINMUX_LEGACY2 \
        xstr(TEST_DEVICE_6000C000_PINMUX_DEVICE2_0_RX_TX_PIN_0)
#define TEST_PINMUX_LEGACY3 \
        xstr(TEST_DEVICE_6000C000_PINMUX_DEVICE2_0_RX_TX_PIN_1)

	zassert(assert_strcmp(TEST_PINMUX_LEGACY0, "[2345,6789]"), 
                "pass", "fail");
	zassert(assert_strcmp(TEST_PINMUX_LEGACY1, "[1234,5678]"),
                "pass", "fail");
	zassert(assert_strcmp(TEST_PINMUX_LEGACY2, "tx"), "pass", "fail");
	zassert(assert_strcmp(TEST_PINMUX_LEGACY3, "rx"), "pass", "fail");
}

void test_pinctrl_pinmux_default_init(void)
{
#define TEST_PINCTRL_PINMUX_INIT(label, idx) \
        _TEST_PINCTRL_PINMUX_INIT1(\
                label##_PINCTRL_##idx##_PINMUX,\
                label##_PINCTRL_##idx##_BIAS_PULL_UP, \
                label##_PINCTRL_##idx##_BIAS_PULL_DOWN, \
                label##_PINCTRL_##idx##_INPUT_ENABLE, \
                label##_PINCTRL_##idx##_OUTPUT_ENABLE)
#define _TEST_PINCTRL_PINMUX_INIT1(_mux, _up, _down, _in, _out) \
        _TEST_PINCTRL_PINMUX_INIT2(_mux, _up, _down, _in, _out)
#define _TEST_PINCTRL_PINMUX_INIT2(_mux0, _mux1, _up, _down, _in, _out) { \
                .pinmux = {_mux0, _mux1}, \
                .bias_pull_up = _up, \
                .bias_pull_down = _down, \
                .input_enable = _in, \
                .output_enable = _out},

        struct s_pinctrl_init {
                u32_t pinmux[2];
                u32_t bias_pull_up;
                u32_t bias_pull_down;
                _Bool input_enable;
                _Bool output_enable;
        };

        const struct s_pinctrl_init pinctrl_init[] = {
#if TEST_PINMUX_40000000_PINCTRL_COUNT > 0
                TEST_PINCTRL_PINMUX_INIT(TEST_PINMUX_40000000, 000)
#endif
#if TEST_PINMUX_40000000_PINCTRL_COUNT > 1
                TEST_PINCTRL_PINMUX_INIT(TEST_PINMUX_40000000, 001)
#endif
                /* unused default configuration index 002 */
#if TEST_PINMUX_40000000_PINCTRL_COUNT > 2
                TEST_PINCTRL_PINMUX_INIT(TEST_PINMUX_40000000, 002)
#endif
        };

        /* http://www.geeksforgeeks.org/how-to-find-size-of-array-in-cc\
         * -without-using-sizeof-operator/ */
        const int pinctrl_init_size = *(&pinctrl_init + 1) - pinctrl_init;

        zassert_equal(TEST_PINMUX_40000000_PINCTRL_COUNT, 2,
                      "pin control init size");
        zassert_equal(pinctrl_init_size, TEST_PINMUX_40000000_PINCTRL_COUNT,
                      "pin control init size");
        zassert_equal(pinctrl_init[0].bias_pull_up,
                      TEST_PINMUX_40000000_PINCTRL_000_BIAS_PULL_UP,
                      "");
        zassert_equal(pinctrl_init[0].bias_pull_down,
                      TEST_PINMUX_40000000_PINCTRL_000_BIAS_PULL_DOWN,
                      "");
        zassert_equal(pinctrl_init[0].input_enable,
                      TEST_PINMUX_40000000_PINCTRL_000_INPUT_ENABLE,
                      "");
        zassert_equal(pinctrl_init[0].output_enable,
                      TEST_PINMUX_40000000_PINCTRL_000_OUTPUT_ENABLE,
                      "");
        zassert_equal(pinctrl_init[1].bias_pull_up,
                      TEST_PINMUX_40000000_PINCTRL_001_BIAS_PULL_UP,
                      "");
        zassert_equal(pinctrl_init[1].bias_pull_down,
                      TEST_PINMUX_40000000_PINCTRL_001_BIAS_PULL_DOWN,
                      "");
        zassert_equal(pinctrl_init[1].input_enable,
                      TEST_PINMUX_40000000_PINCTRL_001_INPUT_ENABLE,
                      "");
        zassert_equal(pinctrl_init[1].output_enable,
                      TEST_PINMUX_40000000_PINCTRL_001_OUTPUT_ENABLE,
                      "");

}

void test_pinctrl_pins_default_init(void)
{
#define PIN_A 23
#define PIN_B 34

#define TEST_PINCTRL_PINS_INIT(label, idx) \
        _TEST_PINCTRL_PINS_INIT1(\
                label##_PINCTRL_##idx##_OUTPUT_HIGH, \
                label##_PINCTRL_##idx##_PINS)
#define _TEST_PINCTRL_PINS_INIT1(_high, _pins) \
        _TEST_PINCTRL_PINS_INIT2(_high, _pins, 0, 0, 0, 0, 0, 0) 
#define _TEST_PINCTRL_PINS_INIT2(_high, _pin0, _pin1, _pin2, _pin3, _pin4, \
                                 _pin5, ...) { \
                .pins = {_pin0, _pin1, _pin2, _pin3, _pin4, _pin5}, \
                .output_high = _high},

        struct s_pinctrl_init {
                u32_t pins[6];
                _Bool output_high;
        };

        const struct s_pinctrl_init pinctrl_init[] = {
#if TEST_PINMUX_50000000_PINCTRL_COUNT > 0
                TEST_PINCTRL_PINS_INIT(TEST_PINMUX_50000000, 000)
#endif
                /* unused default configuration index 001 */
#if TEST_PINMUX_50000000_PINCTRL_COUNT > 1
                TEST_PINCTRL_PINS_INIT(TEST_PINMUX_50000000, 001)
#endif
#if TEST_PINMUX_50000000_PINCTRL_COUNT > 2
                TEST_PINCTRL_PINS_INIT(TEST_PINMUX_50000000, 002)
#endif
        };

        const int pinctrl_init_size = *(&pinctrl_init + 1) - pinctrl_init;

        zassert_equal(TEST_PINMUX_50000000_PINCTRL_COUNT, 1,
                      "pin control init size");
        zassert_equal(pinctrl_init_size, TEST_PINMUX_50000000_PINCTRL_COUNT,
                      "pin control init size");
        zassert_equal(pinctrl_init[0].pins[0], PIN_A, "");
        zassert_equal(pinctrl_init[0].pins[1], PIN_B, "");
        zassert_equal(pinctrl_init[0].pins[2], 0, "");
        zassert_equal(pinctrl_init[0].output_high,
                      TEST_PINMUX_50000000_PINCTRL_000_OUTPUT_HIGH, "");
}

void test_main(void)
{
	ztest_test_suite(test_extract_dts_includes,
                         ztest_unit_test(test_pinctrl_define_directives),
                         ztest_unit_test(test_pinctrl_pinmux_default_init),
                         ztest_unit_test(test_pinctrl_pins_default_init)
                        );
	ztest_run_test_suite(test_extract_dts_includes);
}
