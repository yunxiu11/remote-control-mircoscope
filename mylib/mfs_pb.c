/******************************************************************************
 * @file    mfs_pb.c
 * @author  Yuxuan Zhou
 * @date    18/05/2025
 * @brief   initialise MFS pushbutton
 ******************************************************************************/

#include "stm32f4xx_hal.h"
#include "processor_hal.h"
#include "board.h"
#include "mfs_pb.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "debug_log.h"

SemaphoreHandle_t SemaphoreS1;
SemaphoreHandle_t SemaphoreS2;
SemaphoreHandle_t SemaphoreS3;
SemaphoreHandle_t onboardBtnSemaphore;

uint32_t last_debounce_time[3] = {0};
const uint32_t debounce_delay = 200;

void reg_mfs_pb_init(int pbSelect)
{
    __GPIOC_CLK_ENABLE();
    __GPIOF_CLK_ENABLE();
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    if (pbSelect == 0)
    {
        // PC0 -> S1 -> EXTI0
        GPIOC->MODER &= ~(0x03 << (0 * 2));
        GPIOC->PUPDR &= ~(0x03 << (0 * 2));
        GPIOC->PUPDR |= (0x02 << (0 * 2));

        SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
        SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PC;

        EXTI->RTSR |= EXTI_RTSR_TR0;
        EXTI->FTSR &= ~EXTI_FTSR_TR0;
        EXTI->IMR |= EXTI_IMR_IM0;

        HAL_NVIC_SetPriority(EXTI0_IRQn, 10, 0);
        HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    }
    else if (pbSelect == 2)
    {
        // PF3 -> S3 -> EXTI3
        GPIOF->MODER &= ~(0x03 << (3 * 2));
        GPIOF->PUPDR &= ~(0x03 << (3 * 2));
        GPIOF->PUPDR |= (0x02 << (3 * 2));

        SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3;
        SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PF;

        EXTI->RTSR |= EXTI_RTSR_TR3;
        EXTI->FTSR &= ~EXTI_FTSR_TR3;
        EXTI->IMR |= EXTI_IMR_IM3;

        HAL_NVIC_SetPriority(EXTI3_IRQn, 10, 0);
        HAL_NVIC_EnableIRQ(EXTI3_IRQn);
    }
    else if (pbSelect == 3)
    {
        // PC13
        GPIOC->MODER &= ~(0x03 << (13 * 2));
        GPIOC->PUPDR &= ~(0x03 << (13 * 2));
        GPIOC->PUPDR |= (0x02 << (13 * 2));

        SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
        SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

        EXTI->RTSR |= EXTI_RTSR_TR13;  // Rising edge
        EXTI->FTSR &= ~EXTI_FTSR_TR13; // Disable falling edge
        EXTI->IMR |= EXTI_IMR_IM13;    // Enable interrupt line

        HAL_NVIC_SetPriority(EXTI15_10_IRQn, 10, 0);
        HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    }
}

void reg_mfs_pb_isr(int pbSelect)
{
    uint32_t current_time = HAL_GetTick();

    if ((current_time - last_debounce_time[pbSelect]) > debounce_delay)
    {
        MfPbPressCounter[pbSelect]++;

        if (pbSelect == 0)
        {
            BRD_LEDBlueToggle();
        }
        else if (pbSelect == 2)
        {
            BRD_LEDRedToggle();
        }
        else if (pbSelect == 3)
        {
            BRD_LEDGreenToggle();
        }

        if (MfPbPressCounter[pbSelect] > 7)
        {
            MfPbPressCounter[pbSelect] = 0;
        }

        last_debounce_time[pbSelect] = current_time;

        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        if (pbSelect == 0 && SemaphoreS1 != NULL)
        {
            xSemaphoreGiveFromISR(SemaphoreS1, &xHigherPriorityTaskWoken);
        }
        else if (pbSelect == 2 && SemaphoreS3 != NULL)
        {
            xSemaphoreGiveFromISR(SemaphoreS3, &xHigherPriorityTaskWoken);
        }
        else if (onboardBtnSemaphore != NULL)
        {
            xSemaphoreGiveFromISR(onboardBtnSemaphore, &xHigherPriorityTaskWoken);
        }

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

int reg_mfs_pb_press_get(int pbSelect)
{
    if (pbSelect == 0 || pbSelect == 2)
        return MfPbPressCounter[pbSelect];
    return 0;
}

void reg_mfs_pb_press_reset(int pbSelect)
{
    if (pbSelect == 0 || pbSelect == 2)
        MfPbPressCounter[pbSelect] = 0;
}

void TaskMfsPbInit(void)
{
    SemaphoreS1 = xSemaphoreCreateBinary();
    SemaphoreS3 = xSemaphoreCreateBinary();
    onboardBtnSemaphore = xSemaphoreCreateBinary();

    reg_mfs_pb_init(0);
    reg_mfs_pb_init(2);
    reg_mfs_pb_init(3);
}

void EXTI0_IRQHandler(void)
{
    NVIC_ClearPendingIRQ(EXTI0_IRQn);
    if ((EXTI->PR & EXTI_PR_PR0) == EXTI_PR_PR0)
    {
        EXTI->PR |= EXTI_PR_PR0;
        reg_mfs_pb_isr(0);
    }
}

void EXTI3_IRQHandler(void)
{
    NVIC_ClearPendingIRQ(EXTI3_IRQn);
    if ((EXTI->PR & EXTI_PR_PR3) == EXTI_PR_PR3)
    {
        EXTI->PR |= EXTI_PR_PR3;
        reg_mfs_pb_isr(2);
    }
}

void EXTI15_10_IRQHandler(void)
{
    NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
    if ((EXTI->PR & EXTI_PR_PR13) == EXTI_PR_PR13)
    {
        EXTI->PR |= EXTI_PR_PR13;
        reg_mfs_pb_isr(3);
    }
}
