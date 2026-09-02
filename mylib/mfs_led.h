/**
**************************************************************
* File: mylib/mfs_led.c
* Author: Yuxuan Zhou
* Date: 02/04/2025
* Brief: mylib template driver
***************************************************************
* EXTERNAL FUNCTIONS
* reg_mfs_led_init() - Initialise MFS LED
***************************************************************
*
***************************************************************
*/

#ifndef MFS_LED_H
#define MFS_LED_H

#include "stm32f4xx_hal.h"

void reg_mfs_led_init(void);

// D1 - PA5
#define REG_MFS_LED_D1_ON() (GPIOA->ODR &= ~(1 << 5))
#define REG_MFS_LED_D1_OFF() (GPIOA->ODR |= (1 << 5))

// D2 - PA6
#define REG_MFS_LED_D2_ON() (GPIOA->ODR &= ~(1 << 6))
#define REG_MFS_LED_D2_OFF() (GPIOA->ODR |= (1 << 6))

// D3 - PA7
#define REG_MFS_LED_D3_ON() (GPIOA->ODR &= ~(1 << 7))
#define REG_MFS_LED_D3_OFF() (GPIOA->ODR |= (1 << 7))

// D4 - PD14
#define REG_MFS_LED_D4_ON() (GPIOD->ODR &= ~(1 << 14))
#define REG_MFS_LED_D4_OFF() (GPIOD->ODR |= (1 << 14))

#endif
