/******************************************************************************
 * @file    s4831567_lta1000g.c
 * @author  Yuxuan Zhou
 * @date    14/03/2025
 * @brief   LTA1000G LED Bar driver
 ******************************************************************************
 */

#include "stm32f4xx_hal.h"
#include "processor_hal.h"
#include "board.h"

void s4831567_reg_lta1000g_init()
{

    __GPIOG_CLK_ENABLE();
    __GPIOE_CLK_ENABLE();
    __GPIOF_CLK_ENABLE();
    // Initialise G5 as output
    GPIOG->MODER &= ~(0x03 << (5 * 2)); // Clear bits
    GPIOG->MODER |= (0x01 << (5 * 2));  // Set for push-pull
    GPIOG->OSPEEDR &= ~(0x03 << (5 * 2));
    GPIOG->OSPEEDR |= (0x02 << (5 * 2)); // Set for Fast speed
    GPIOG->OTYPER &= ~(0x01 << 5);       // Clear Bit for Push/Pull Output
    GPIOG->PUPDR &= ~(0x03 << (5 * 2));  // Clear Bits
    GPIOG->PUPDR |= (0x01 << (5 * 2));   // Set for Pull down output

    // Initialise G6 as output
    GPIOG->MODER &= ~(0x03 << (6 * 2)); // Clear bits
    GPIOG->MODER |= (0x01 << (6 * 2));  // Set for push-pull
    GPIOG->OSPEEDR &= ~(0x03 << (6 * 2));
    GPIOG->OSPEEDR |= (0x02 << (6 * 2)); // Set for Fast speed
    GPIOG->OTYPER &= ~(0x01 << 6);       // Clear Bit for Push/Pull Output
    GPIOG->PUPDR &= ~(0x03 << (6 * 2));  // Clear Bits
    GPIOG->PUPDR |= (0x01 << (6 * 2));   // Set for Pull down output

    // Initialise G8 as output
    GPIOG->MODER &= ~(0x03 << (8 * 2)); // Clear bits
    GPIOG->MODER |= (0x01 << (8 * 2));  // Set for push-pull
    GPIOG->OSPEEDR &= ~(0x03 << (8 * 2));
    GPIOG->OSPEEDR |= (0x02 << (8 * 2)); // Set for Fast speed
    GPIOG->OTYPER &= ~(0x01 << 8);       // Clear Bit for Push/Pull Output
    GPIOG->PUPDR &= ~(0x03 << (8 * 2));  // Clear Bits
    GPIOG->PUPDR |= (0x01 << (8 * 2));   // Set for Pull down output

    // Initialise G10 as output
    GPIOG->MODER &= ~(0x03 << (10 * 2)); // Clear bits
    GPIOG->MODER |= (0x01 << (10 * 2));  // Set for push-pull
    GPIOG->OSPEEDR &= ~(0x03 << (10 * 2));
    GPIOG->OSPEEDR |= (0x02 << (10 * 2)); // Set for Fast speed
    GPIOG->OTYPER &= ~(0x01 << 10);       // Clear Bit for Push/Pull Output
    GPIOG->PUPDR &= ~(0x03 << (10 * 2));  // Clear Bits
    GPIOG->PUPDR |= (0x01 << (10 * 2));   // Set for Pull down output

    // Initialise G11 as output
    GPIOG->MODER &= ~(0x03 << (11 * 2)); // Clear bits
    GPIOG->MODER |= (0x01 << (11 * 2));  // Set for push-pull
    GPIOG->OSPEEDR &= ~(0x03 << (11 * 2));
    GPIOG->OSPEEDR |= (0x02 << (11 * 2)); // Set for Fast speed
    GPIOG->OTYPER &= ~(0x01 << 11);       // Clear Bit for Push/Pull Output
    GPIOG->PUPDR &= ~(0x03 << (11 * 2));  // Clear Bits
    GPIOG->PUPDR |= (0x01 << (11 * 2));   // Set for Pull down output

    // Initialise G13 as output
    GPIOG->MODER &= ~(0x03 << (13 * 2)); // Clear bits
    GPIOG->MODER |= (0x01 << (13 * 2));  // Set for push-pull
    GPIOG->OSPEEDR &= ~(0x03 << (13 * 2));
    GPIOG->OSPEEDR |= (0x02 << (13 * 2)); // Set for Fast speed
    GPIOG->OTYPER &= ~(0x01 << 13);       // Clear Bit for Push/Pull Output
    GPIOG->PUPDR &= ~(0x03 << (13 * 2));  // Clear Bits
    GPIOG->PUPDR |= (0x01 << (13 * 2));   // Set for Pull down output

    // Initialise G15 as output
    GPIOG->MODER &= ~(0x03 << (15 * 2)); // Clear bits
    GPIOG->MODER |= (0x01 << (15 * 2));  // Set for push-pull
    GPIOG->OSPEEDR &= ~(0x03 << (15 * 2));
    GPIOG->OSPEEDR |= (0x02 << (15 * 2)); // Set for Fast speed
    GPIOG->OTYPER &= ~(0x01 << 15);       // Clear Bit for Push/Pull Output
    GPIOG->PUPDR &= ~(0x03 << (15 * 2));  // Clear Bits
    GPIOG->PUPDR |= (0x01 << (15 * 2));   // Set for Pull down output

    // Initialise E0 as output
    GPIOE->MODER &= ~(0x03 << (0 * 2)); // Clear bits
    GPIOE->MODER |= (0x01 << (0 * 2));  // Set for push-pull
    GPIOE->OSPEEDR &= ~(0x03 << (0 * 2));
    GPIOE->OSPEEDR |= (0x02 << (0 * 2)); // Set for Fast speed
    GPIOE->OTYPER &= ~(0x01 << 0);       // Clear Bit for Push/Pull Output
    GPIOE->PUPDR &= ~(0x03 << (0 * 2));  // Clear Bits
    GPIOE->PUPDR |= (0x01 << (0 * 2));   // Set for Pull down output

    // Initialise E6 as output
    GPIOE->MODER &= ~(0x03 << (6 * 2)); // Clear bits
    GPIOE->MODER |= (0x01 << (6 * 2));  // Set for push-pull
    GPIOE->OSPEEDR &= ~(0x03 << (6 * 2));
    GPIOE->OSPEEDR |= (0x02 << (6 * 2)); // Set for Fast speed
    GPIOE->OTYPER &= ~(0x01 << 6);       // Clear Bit for Push/Pull Output
    GPIOE->PUPDR &= ~(0x03 << (6 * 2));  // Clear Bits
    GPIOE->PUPDR |= (0x01 << (6 * 2));   // Set for Pull down output

    // Initialise F11 as output
    GPIOF->MODER &= ~(0x03 << (11 * 2)); // Clear bits
    GPIOF->MODER |= (0x01 << (11 * 2));  // Set for push-pull
    GPIOF->OSPEEDR &= ~(0x03 << (11 * 2));
    GPIOF->OSPEEDR |= (0x02 << (11 * 2)); // Set for Fast speed
    GPIOF->OTYPER &= ~(0x01 << 11);       // Clear Bit for Push/Pull Output
    GPIOF->PUPDR &= ~(0x03 << (11 * 2));  // Clear Bits
    GPIOF->PUPDR |= (0x01 << (11 * 2));   // Set for Pull down output
}

void lta1000g_seg_set(int segment, unsigned char value)
{

    if (segment == 0)
    {
        if (value)
        {
            GPIOE->ODR |= (1 << 6); // Set the segment high
        }
        else
        {
            GPIOE->ODR &= ~(1 << 6); // Set the segment low
        }
    }
    else if (segment == 1)
    {
        if (value)
        {
            GPIOG->ODR |= (1 << 15); // Set the segment high
        }
        else
        {
            GPIOG->ODR &= ~(1 << 15); // Set the segment low
        }
    }
    else if (segment == 2)
    {
        if (value)
        {
            GPIOG->ODR |= (1 << 10); // Set the segment high
        }
        else
        {
            GPIOG->ODR &= ~(1 << 10); // Set the segment low
        }
    }
    else if (segment == 3)
    {
        if (value)
        {
            GPIOG->ODR |= (1 << 13); // Set the segment high
        }
        else
        {
            GPIOG->ODR &= ~(1 << 13); // Set the segment low
        }
    }
    else if (segment == 4)
    {
        if (value)
        {
            GPIOG->ODR |= (1 << 11); // Set the segment high
        }
        else
        {
            GPIOG->ODR &= ~(1 << 11); // Set the segment low
        }
    }
    else if (segment == 5)
    {
        if (value)
        {
            GPIOF->ODR |= (1 << 11); // Set the segment high
        }
        else
        {
            GPIOF->ODR &= ~(1 << 11); // Set the segment low
        }
    }
    else if (segment == 6)
    {
        if (value)
        {
            GPIOE->ODR |= (1 << 0); // Set the segment high
        }
        else
        {
            GPIOE->ODR &= ~(1 << 0); // Set the segment low
        }
    }
    else if (segment == 7)
    {
        if (value)
        {
            GPIOG->ODR |= (1 << 8); // Set the segment high
        }
        else
        {
            GPIOG->ODR &= ~(1 << 8); // Set the segment low
        }
    }
    else if (segment == 8)
    {
        if (value)
        {
            GPIOG->ODR |= (1 << 5); // Set the segment high
        }
        else
        {
            GPIOG->ODR &= ~(1 << 5); // Set the segment low
        }
    }
    else if (segment == 9)
    {
        if (value)
        {
            GPIOG->ODR |= (1 << 6); // Set the segment high
        }
        else
        {
            GPIOG->ODR &= ~(1 << 6); // Set the segment low
        }
    }
}

void s4831567_reg_lta1000g_write(unsigned short value)
{
    for (int i = 0; i < 10; i++)
    {
        unsigned char bit_value = (value >> i) & 0x001;
        lta1000g_seg_set(i, bit_value);
    }
}

unsigned short s4831567_reg_lta1000g_brotate(unsigned short value, int direction)
{
    unsigned short new_value;
    if (direction == 0)
    {
        // Left rotate
        new_value = (value << 1) | (value >> 9);
    }
    else
    {
        // Right rotate
        new_value = (value >> 1) | ((value & 0x001) << 9);
    }
    s4831567_reg_lta1000g_write(new_value);
    return new_value;
}