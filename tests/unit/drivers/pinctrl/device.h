
/* auto-generated by gen_syscalls.py, don't edit */

#ifndef _ASMLANGUAGE

#include <syscall_list.h>
#include <syscall_macros.h>

#if 0 /* We just need the file as dummy */

#ifdef __cplusplus
extern "C" {
#endif

K_SYSCALL_DECLARE1(K_SYSCALL_PINCTRL_GET_PINS_COUNT, pinctrl_get_pins_count, u16_t, struct device *, dev)

K_SYSCALL_DECLARE1(K_SYSCALL_PINCTRL_GET_GROUPS_COUNT, pinctrl_get_groups_count, u16_t, struct device *, dev)

K_SYSCALL_DECLARE4(K_SYSCALL_PINCTRL_GET_GROUP_PINS, pinctrl_get_group_pins, int, struct device *, dev, u16_t, group, u16_t *, pins, u16_t *, num_pins)

K_SYSCALL_DECLARE1(K_SYSCALL_PINCTRL_GET_STATES_COUNT, pinctrl_get_states_count, u16_t, struct device *, dev)

K_SYSCALL_DECLARE3(K_SYSCALL_PINCTRL_GET_STATE_GROUP, pinctrl_get_state_group, int, struct device *, dev, u16_t, state, u16_t *, group)

K_SYSCALL_DECLARE1(K_SYSCALL_PINCTRL_GET_FUNCTIONS_COUNT, pinctrl_get_functions_count, u16_t, struct device *, dev)

K_SYSCALL_DECLARE4(K_SYSCALL_PINCTRL_GET_FUNCTION_GROUP, pinctrl_get_function_group, int, struct device *, dev, u16_t, func, const char *, name, u16_t *, group)

K_SYSCALL_DECLARE4(K_SYSCALL_PINCTRL_GET_FUNCTION_GROUPS, pinctrl_get_function_groups, int, struct device *, dev, u16_t, func, u16_t *, groups, u16_t *, num_groups)

K_SYSCALL_DECLARE4(K_SYSCALL_PINCTRL_GET_FUNCTION_STATE, pinctrl_get_function_state, int, struct device *, dev, u16_t, func, const char *, name, u16_t *, state)

K_SYSCALL_DECLARE4(K_SYSCALL_PINCTRL_GET_FUNCTION_STATES, pinctrl_get_function_states, int, struct device *, dev, u16_t, func, u16_t *, states, u16_t *, num_states)

K_SYSCALL_DECLARE3(K_SYSCALL_PINCTRL_GET_DEVICE_FUNCTION, pinctrl_get_device_function, int, struct device *, dev, struct device *, client, u16_t *, func)

K_SYSCALL_DECLARE6(K_SYSCALL_PINCTRL_GET_GPIO_RANGE, pinctrl_get_gpio_range, int, struct device *, dev, struct device *, gpio, u32_t, gpio_pin, u16_t *, pin, u16_t *, base_pin, u8_t *, num_pins)

K_SYSCALL_DECLARE3(K_SYSCALL_PINCTRL_CONFIG_GET, pinctrl_config_get, int, struct device *, dev, u16_t, pin, u32_t *, config)

K_SYSCALL_DECLARE3(K_SYSCALL_PINCTRL_CONFIG_SET, pinctrl_config_set, int, struct device *, dev, u16_t, pin, u32_t, config)

K_SYSCALL_DECLARE4(K_SYSCALL_PINCTRL_CONFIG_GROUP_GET, pinctrl_config_group_get, int, struct device *, dev, u16_t, group, u32_t *, configs, u16_t *, num_configs)

K_SYSCALL_DECLARE4(K_SYSCALL_PINCTRL_CONFIG_GROUP_SET, pinctrl_config_group_set, int, struct device *, dev, u16_t, group, const u32_t *, configs, u16_t, num_configs)

K_SYSCALL_DECLARE3(K_SYSCALL_PINCTRL_MUX_REQUEST, pinctrl_mux_request, int, struct device *, dev, u16_t, pin, const char *, owner)

K_SYSCALL_DECLARE3(K_SYSCALL_PINCTRL_MUX_FREE, pinctrl_mux_free, int, struct device *, dev, u16_t, pin, const char *, owner)

K_SYSCALL_DECLARE3(K_SYSCALL_PINCTRL_MUX_GET, pinctrl_mux_get, int, struct device *, dev, u16_t, pin, u16_t *, func)

K_SYSCALL_DECLARE3(K_SYSCALL_PINCTRL_MUX_SET, pinctrl_mux_set, int, struct device *, dev, u16_t, pin, u16_t, func)

K_SYSCALL_DECLARE3(K_SYSCALL_PINCTRL_MUX_GROUP_SET, pinctrl_mux_group_set, int, struct device *, dev, u16_t, group, u16_t, func)

K_SYSCALL_DECLARE2(K_SYSCALL_PINCTRL_STATE_SET, pinctrl_state_set, int, struct device *, dev, u16_t, state)

#ifdef __cplusplus
}
#endif
#endif

#endif
