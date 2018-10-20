/*
 * Copyright (c) 2017 RnDity Sp. z o.o.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file SoC configuration macros for the STM32F0 family processors.
 *
 * Based on reference manual:
 *   STM32F030x4/x6/x8/xC,
 *   STM32F070x6/xB advanced ARM ® -based MCUs
 *
 * Chapter 2.2: Memory organization
 */


#ifndef _STM32F0_SOC_H_
#define _STM32F0_SOC_H_

#ifndef _ASMLANGUAGE

#include <stm32f0xx.h>

/* ARM CMSIS definitions must be included before kernel_includes.h.
 * Therefore, it is essential to include kernel_includes.h after including
 * core SOC-specific headers.
 */
#include <kernel_includes.h>

/* Include LL headers configured by Kconfig. */

#if CONFIG_USE_STM32_LL_ADC
#include <stm32f0xx_ll_adc.h>
#endif

#if CONFIG_USE_STM32_LL_BUS
#include <stm32f0xx_ll_bus.h>
#endif

#if CONFIG_USE_STM32_LL_COMP
#include <stm32f0xx_ll_comp.h>
#endif

#if CONFIG_USE_STM32_LL_CRC
#include <stm32f0xx_ll_crc.h>
#endif

#if CONFIG_USE_STM32_LL_CRS
#include <stm32f0xx_ll_crs.h>
#endif

#if CONFIG_USE_STM32_LL_DAC
#include <stm32f0xx_ll_dac.h>
#endif

#if CONFIG_USE_STM32_LL_DMA
#include <stm32f0xx_ll_dma.h>
#endif

#if CONFIG_USE_STM32_LL_DMA2D
#endif

#if CONFIG_USE_STM32_LL_EXTI
#include <stm32f0xx_ll_exti.h>
#endif

#if CONFIG_USE_STM32_LL_FMC
#endif

#if CONFIG_USE_STM32_LL_FSMC
#endif

#if CONFIG_USE_STM32_LL_GPIO
#include <stm32f0xx_ll_gpio.h>
#endif

#if CONFIG_USE_STM32_LL_HRTIM
#endif

#if CONFIG_USE_STM32_LL_I2C
#include <stm32f0xx_ll_i2c.h>
#endif

#if CONFIG_USE_STM32_LL_IWDG
#include <stm32f0xx_ll_iwdg.h>
#endif

#if CONFIG_USE_STM32_LL_LPTIM
#endif

#if CONFIG_USE_STM32_LL_LPUART
#endif

#if CONFIG_USE_STM32_LL_OPAMP
#endif

#if CONFIG_USE_STM32_LL_PWR
#include <stm32f0xx_ll_pwr.h>
#endif

#if CONFIG_USE_STM32_LL_RCC
#include <stm32f0xx_ll_rcc.h>
#endif

#if CONFIG_USE_STM32_LL_RNG
#endif

#if CONFIG_USE_STM32_LL_RTC
#include <stm32f0xx_ll_rtc.h>
#endif

#if CONFIG_USE_STM32_LL_SDMMC
#endif

#if CONFIG_USE_STM32_LL_SPI
#include <stm32f0xx_ll_spi.h>
#endif

#if CONFIG_USE_STM32_LL_SWPMI
#endif

#if CONFIG_USE_STM32_LL_SYSTEM
#include <stm32f0xx_ll_system.h>
#endif

#if CONFIG_USE_STM32_LL_TIM
#include <stm32f0xx_ll_tim.h>
#endif

#if CONFIG_USE_STM32_LL_USART
#include <stm32f0xx_ll_usart.h>
#endif

#if CONFIG_USE_STM32_LL_USB
#endif

#if CONFIG_USE_STM32_LL_UTILS
#include <stm32f0xx_ll_utils.h>
#endif

#if CONFIG_USE_STM32_LL_WWDG
#include <stm32f0xx_ll_wwdg.h>
#endif

#endif /* !_ASMLANGUAGE */

#endif /* _STM32F0_SOC_H_ */
