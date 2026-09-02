/******************************************************************************
 * @file    rgb.c
 * @author  Yuxuan Zhou
 * @date    19/05/2025
 * @brief   RGB LED GPIO control driver (no PWM)
 ******************************************************************************/

#include "stm32f4xx_hal.h"
#include "processor_hal.h"
#include "board.h"

void reg_rgb_init(void)
{
    __GPIOE_CLK_ENABLE();
    __GPIOF_CLK_ENABLE();
    __GPIOA_CLK_ENABLE();

    // --- E11 (R) ---
    GPIOE->MODER &= ~(0x03 << (11 * 2));
    GPIOE->MODER |= (0x01 << (11 * 2));
    GPIOE->OSPEEDR &= ~(0x03 << (11 * 2));
    GPIOE->OSPEEDR |= (0x02 << (11 * 2));
    GPIOE->OTYPER &= ~(0x01 << 11);
    GPIOE->PUPDR &= ~(0x03 << (11 * 2));
    GPIOE->PUPDR |= (0x01 << (11 * 2));

    // --- A12 (G) ---
    GPIOA->MODER &= ~(0x03 << (12 * 2));
    GPIOA->MODER |= (0x01 << (12 * 2));
    GPIOA->OSPEEDR &= ~(0x03 << (12 * 2));
    GPIOA->OSPEEDR |= (0x02 << (12 * 2));
    GPIOA->OTYPER &= ~(0x01 << 12);
    GPIOA->PUPDR &= ~(0x03 << (12 * 2));
    GPIOA->PUPDR |= (0x01 << (12 * 2));

    // --- F15 (B) ---
    GPIOF->MODER &= ~(0x03 << (15 * 2));
    GPIOF->MODER |= (0x01 << (15 * 2));
    GPIOF->OSPEEDR &= ~(0x03 << (15 * 2));
    GPIOF->OSPEEDR |= (0x02 << (15 * 2));
    GPIOF->OTYPER &= ~(0x01 << 15);
    GPIOF->PUPDR &= ~(0x03 << (15 * 2));
    GPIOF->PUPDR |= (0x01 << (15 * 2));
}

void reg_rgb_colour_set(unsigned char rgb_mask)
{
    // --- RED ---
    if (rgb_mask & 0x04)
    {
        GPIOE->ODR |= (1 << 11); // E11 HIGH
    }
    else
    {
        GPIOE->ODR &= ~(1 << 11); // E11 LOW
    }

    // --- GREEN ---
    if (rgb_mask & 0x02)
    {
        GPIOA->ODR |= (1 << 12); // A12 HIGH
    }
    else
    {
        GPIOA->ODR &= ~(1 << 12); // A12 LOW
    }

    // --- BLUE ---
    if (rgb_mask & 0x01)
    {
        GPIOF->ODR |= (1 << 15); // F15 HIGH
    }
    else
    {
        GPIOF->ODR &= ~(1 << 15); // F15 LOW
    }
}
