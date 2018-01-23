# Board initialization
zephyr_sources_ifdef(CONFIG_PINCTRL	pinctrl_stm32.c)

zephyr_sources_ifdef(CONFIG_USERSPACE  	pinctrl_handlers.c)
