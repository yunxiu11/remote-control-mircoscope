/******************************************************************************
 * @file    switchbank.c
 * @author  Yuxuan Zhou
 * @date    22/04/2025
 * @brief   initialise MFS LED
 ******************************************************************************/

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "stm32f4xx_hal.h"
#include "processor_hal.h"
#include "board.h"

QueueHandle_t QueueSwitchbank;

typedef struct
{
    uint8_t switch_val;
} SwitchbankMsg;

void reg_switchbank_init()
{
    __GPIOE_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();

    // Initialise E8 as input
    GPIOE->MODER &= ~(0x03 << (8 * 2)); // Clear bits for input mode
    GPIOE->OSPEEDR &= ~(0x03 << (8 * 2));
    GPIOE->OSPEEDR |= 0x02 << (8 * 2);   // Fast speed
    GPIOE->PUPDR &= ~(0x03 << (8 * 2));  // Clear bits for no push/pull
    GPIOE->PUPDR |= ((0x02) << (8 * 2)); // Set for Pull down output

    // Initialise E7 as input
    GPIOE->MODER &= ~(0x03 << (7 * 2)); // Clear bits for input mode
    GPIOE->OSPEEDR &= ~(0x03 << (7 * 2));
    GPIOE->OSPEEDR |= 0x02 << (7 * 2);   // Fast speed
    GPIOE->PUPDR &= ~(0x03 << (7 * 2));  // Clear bits for no push/pull
    GPIOE->PUPDR |= ((0x02) << (7 * 2)); // Set for Pull down output

    // Initialise E10 as input
    GPIOE->MODER &= ~(0x03 << (10 * 2)); // Clear bits for input mode
    GPIOE->OSPEEDR &= ~(0x03 << (10 * 2));
    GPIOE->OSPEEDR |= 0x02 << (10 * 2);   // Fast speed
    GPIOE->PUPDR &= ~(0x03 << (10 * 2));  // Clear bits for no push/pull
    GPIOE->PUPDR |= ((0x02) << (10 * 2)); // Set for Pull down output

    // Initialise E12 as input
    GPIOE->MODER &= ~(0x03 << (12 * 2)); // Clear bits for input mode
    GPIOE->OSPEEDR &= ~(0x03 << (12 * 2));
    GPIOE->OSPEEDR |= 0x02 << (12 * 2);   // Fast speed
    GPIOE->PUPDR &= ~(0x03 << (12 * 2));  // Clear bits for no push/pull
    GPIOE->PUPDR |= ((0x02) << (12 * 2)); // Set for Pull down output

    // Initialise E14 as input
    GPIOE->MODER &= ~(0x03 << (14 * 2)); // Clear bits for input mode
    GPIOE->OSPEEDR &= ~(0x03 << (14 * 2));
    GPIOE->OSPEEDR |= 0x02 << (14 * 2);   // Fast speed
    GPIOE->PUPDR &= ~(0x03 << (14 * 2));  // Clear bits for no push/pull
    GPIOE->PUPDR |= ((0x02) << (14 * 2)); // Set for Pull down output

    // Initialise E15 as input
    GPIOE->MODER &= ~(0x03 << (15 * 2)); // Clear bits for input mode
    GPIOE->OSPEEDR &= ~(0x03 << (15 * 2));
    GPIOE->OSPEEDR |= 0x02 << (15 * 2);   // Fast speed
    GPIOE->PUPDR &= ~(0x03 << (15 * 2));  // Clear bits for no push/pull
    GPIOE->PUPDR |= ((0x02) << (15 * 2)); // Set for Pull down output

    // Initialise B13 as input
    GPIOB->MODER &= ~(0x03 << (13 * 2)); // Clear bits for input mode
    GPIOB->OSPEEDR &= ~(0x03 << (13 * 2));
    GPIOB->OSPEEDR |= 0x02 << (13 * 2);   // Fast speed
    GPIOB->PUPDR &= ~(0x03 << (13 * 2));  // Clear bits for no push/pull
    GPIOB->PUPDR |= ((0x02) << (13 * 2)); // Set for Pull down output

    // Initialise B11 as input
    GPIOB->MODER &= ~(0x03 << (11 * 2)); // Clear bits for input mode
    GPIOB->OSPEEDR &= ~(0x03 << (11 * 2));
    GPIOB->OSPEEDR |= 0x02 << (11 * 2);   // Fast speed
    GPIOB->PUPDR &= ~(0x03 << (11 * 2));  // Clear bits for no push/pull
    GPIOB->PUPDR |= ((0x02) << (11 * 2)); // Set for Pull down output
}

unsigned char reg_switchbank_read()
{
    unsigned char switch_state = 0;

    // Read GPIOE pins (E7, E8, E10, E12, E14, E15)
    if (GPIOE->IDR & (1 << 8))
    {
        switch_state |= (1 << 0); // E7 -> Switch[0]
    }
    if (GPIOE->IDR & (1 << 7))
    {
        switch_state |= (1 << 1); // E8 -> Switch[1]
    }
    if (GPIOE->IDR & (1 << 10))
    {
        switch_state |= (1 << 2); // E10 -> Switch[2]
    }
    if (GPIOE->IDR & (1 << 12))
    {
        switch_state |= (1 << 3); // E12 -> Switch[3]
    }
    if (GPIOE->IDR & (1 << 14))
    {
        switch_state |= (1 << 4); // E14 -> Switch[4]
    }
    if (GPIOE->IDR & (1 << 15))
    {
        switch_state |= (1 << 5); // E15 -> Switch[5]
    }

    // Read GPIOB pins (B13, B11)
    if (GPIOB->IDR & (1 << 13))
    {
        switch_state |= (1 << 6); // B13 -> Switch[6]
    }
    if (GPIOB->IDR & (1 << 11))
    {
        switch_state |= (1 << 7); // B11 -> Switch[7]
    }

    return switch_state;
}

QueueHandle_t QueueSwitchbank;

void TaskSwitchbank(void)
{
    reg_switchbank_init();
    QueueSwitchbank = xQueueCreate(5, sizeof(SwitchbankMsg));
    SwitchbankMsg msg;

    for (;;)
    {
        msg.switch_val = reg_switchbank_read();
        xQueueSend(QueueSwitchbank, &msg, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void TaskSwitchbankInit(void)
{
    xTaskCreate((void *)&TaskSwitchbank, "SWTask", 256, NULL, 2, NULL);
}