/******************************************************************************
* @file    s4831567_mfs_led.c
* @author  Yuxuan Zhou
* @date    02/04/2025
* @brief   initialise MFS LED
******************************************************************************/

#include "stm32f4xx_hal.h"
#include "processor_hal.h"
#include "board.h"

void s4831567_reg_mfs_led_init() {
    __GPIOA_CLK_ENABLE();
    __GPIOD_CLK_ENABLE();

    // Initialise A5 as output
    GPIOA->MODER &= ~(0x03 << (5 * 2));  // Clear mode bits
    GPIOA->MODER |= (0x01 << (5 * 2));   // Set as output
    GPIOA->OSPEEDR &= ~(0x03 << (5 * 2)); // Clear speed bits
    GPIOA->OSPEEDR |= (0x02 << (5 * 2));  // Set fast speed
    GPIOA->OTYPER &= ~(1 << 5);          // Set push-pull
    GPIOA->PUPDR &= ~(0x03 << (5 * 2));  // Clear pull-up/pull-down bits

    // Initialise A6 as output
    GPIOA->MODER &= ~(0x03 << (6 * 2));  // Clear mode bits
    GPIOA->MODER |= (0x01 << (6 * 2));   // Set as output
    GPIOA->OSPEEDR &= ~(0x03 << (6 * 2)); // Clear speed bits
    GPIOA->OSPEEDR |= (0x02 << (6 * 2));  // Set fast speed
    GPIOA->OTYPER &= ~(1 << 6);          // Set push-pull
    GPIOA->PUPDR &= ~(0x03 << (6 * 2));  // Clear pull-up/pull-down bits

    // Initialise A7 as output
    GPIOA->MODER &= ~(0x03 << (7 * 2));  // Clear mode bits
    GPIOA->MODER |= (0x01 << (7 * 2));   // Set as output
    GPIOA->OSPEEDR &= ~(0x03 << (7 * 2)); // Clear speed bits
    GPIOA->OSPEEDR |= (0x02 << (7 * 2));  // Set fast speed
    GPIOA->OTYPER &= ~(1 << 7);          // Set push-pull
    GPIOA->PUPDR &= ~(0x03 << (7 * 2));  // Clear pull-up/pull-down bits

    // Initialise D14 as output
    GPIOD->MODER &= ~(0x03 << (14 * 2));  // Clear mode bits
    GPIOD->MODER |= (0x01 << (14 * 2));   // Set as output
    GPIOD->OSPEEDR &= ~(0x03 << (14 * 2)); // Clear speed bits
    GPIOD->OSPEEDR |= (0x02 << (14 * 2));  // Set fast speed
    GPIOD->OTYPER &= ~(1 << 14);          // Set push-pull
    GPIOD->PUPDR &= ~(0x03 << (14 * 2));  // Clear pull-up/pull-down bits

    // Set initial state to OFF (high level for low-active LEDs)
    GPIOA->ODR |= (1 << 5);   // D1 OFF
    GPIOA->ODR |= (1 << 6);   // D2 OFF
    GPIOA->ODR |= (1 << 7);   // D3 OFF
    GPIOD->ODR |= (1 << 14);  // D4 OFF
}
