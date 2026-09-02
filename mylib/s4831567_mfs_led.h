/**
**************************************************************
* File: mylib/s4831567_mfs_led.c
* Author: Yuxuan Zhou - s4831567
* Date: 02/04/2025
* Brief: mylib template driver
***************************************************************
* EXTERNAL FUNCTIONS
* s4831567_reg_mfs_led_init() - Initialise MFS LED
***************************************************************
*
***************************************************************
*/

#ifndef S4831567_MFS_LED_H
#define S4831567_MFS_LED_H

#include "stm32f4xx_hal.h"

void s4831567_reg_mfs_led_init(void);

// D1 - PA5
#define S4831567_REG_MFS_LED_D1_ON() (GPIOA->ODR &= ~(1 << 5))
#define S4831567_REG_MFS_LED_D1_OFF() (GPIOA->ODR |= (1 << 5))

// D2 - PA6
#define S4831567_REG_MFS_LED_D2_ON() (GPIOA->ODR &= ~(1 << 6))
#define S4831567_REG_MFS_LED_D2_OFF() (GPIOA->ODR |= (1 << 6))

// D3 - PA7
#define S4831567_REG_MFS_LED_D3_ON() (GPIOA->ODR &= ~(1 << 7))
#define S4831567_REG_MFS_LED_D3_OFF() (GPIOA->ODR |= (1 << 7))

// D4 - PD14
#define S4831567_REG_MFS_LED_D4_ON() (GPIOD->ODR &= ~(1 << 14))
#define S4831567_REG_MFS_LED_D4_OFF() (GPIOD->ODR |= (1 << 14))

#endif
