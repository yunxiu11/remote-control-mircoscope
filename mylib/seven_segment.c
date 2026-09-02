/******************************************************************************
 * @file    seven_segment.c
 * @author  Yuxuan Zhou
 * @date    19/05/2025
 * @brief   Seven segment display GPIO control driver
 ******************************************************************************/
#include "stm32f4xx_hal.h"
#include "processor_hal.h"
#include "board.h"
#include "seven_segment.h"

#define SER_PIN 12   // PF12
#define SRCLK_PIN 13 // PF13
#define RCLK_PIN 14  // PF14

#define SER_HIGH (GPIOF->BSRR = (1 << SER_PIN))
#define SER_LOW (GPIOF->BSRR = (1 << (SER_PIN + 16)))

#define SRCLK_HIGH (GPIOF->BSRR = (1 << SRCLK_PIN))
#define SRCLK_LOW (GPIOF->BSRR = (1 << (SRCLK_PIN + 16)))

#define RCLK_HIGH (GPIOF->BSRR = (1 << RCLK_PIN))
#define RCLK_LOW (GPIOF->BSRR = (1 << (RCLK_PIN + 16)))

#define SEG_MINUS 0x40

const uint8_t seg_digit_encoding[11] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    SEG_MINUS};

void segdisplay_init(void)
{
    __GPIOF_CLK_ENABLE();

    GPIOF->MODER &= ~((0x03 << (SER_PIN * 2)) |
                      (0x03 << (SRCLK_PIN * 2)) |
                      (0x03 << (RCLK_PIN * 2)));
    GPIOF->MODER |= ((0x01 << (SER_PIN * 2)) |
                     (0x01 << (SRCLK_PIN * 2)) |
                     (0x01 << (RCLK_PIN * 2)));

    GPIOF->OTYPER &= ~((1 << SER_PIN) | (1 << SRCLK_PIN) | (1 << RCLK_PIN));
    GPIOF->PUPDR &= ~((0x03 << (SER_PIN * 2)) |
                      (0x03 << (SRCLK_PIN * 2)) |
                      (0x03 << (RCLK_PIN * 2)));
}

void segdisplay_shiftout_dual(uint8_t seg_data, uint8_t digit_sel)
{
    uint16_t combined = (seg_data << 8) | digit_sel;

    for (int i = 15; i >= 0; i--)
    {
        if ((combined >> i) & 0x01)
            SER_HIGH;
        else
            SER_LOW;

        SRCLK_HIGH;
        SRCLK_LOW;
    }

    RCLK_HIGH;
    RCLK_LOW;
}

void segdisplay_write_digit_at(uint8_t digit, uint8_t pos)
{
    if (digit > 9 || pos > 3)
    {
        segdisplay_shiftout_dual(0xFF, ~(1 << pos));
    }
    else
    {
        uint8_t seg_data = ~seg_digit_encoding[digit];
        uint8_t digit_sel = (1 << pos);
        segdisplay_shiftout_dual(seg_data, digit_sel);
    }
}

void segdisplay_write_number(int number)
{
    uint8_t digits[4] = {0};
    int is_negative = 0;

    if (number > 9999)
        number = 9999;
    if (number < -999)
        number = -999;

    if (number < 0)
    {
        is_negative = 1;
        number = -number;
    }

    digits[3] = number % 10;
    digits[2] = (number / 10) % 10;
    digits[1] = (number / 100) % 10;
    digits[0] = (number / 1000) % 10;

    if (is_negative)
    {
        digits[0] = 10;
    }

    for (int pos = 0; pos < 4; pos++)
    {
        uint8_t seg_data;
        if (digits[pos] <= 9)
        {
            seg_data = ~seg_digit_encoding[digits[pos]];
        }
        else
        {
            seg_data = ~SEG_MINUS;
        }

        uint8_t digit_sel = (1 << pos);
        segdisplay_shiftout_dual(seg_data, digit_sel);
        HAL_Delay(2);
    }
}
