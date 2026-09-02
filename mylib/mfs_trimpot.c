/******************************************************************************
* @file    trim_pot.c
* @author  Yuxuan Zhou
* @date    18/05/2025
* @brief   Trimpot ADC driver
* @details This file contains the implementation of the trimpot ADC driver.
*          It initializes the ADC and configures the GPIO pins for the trimpot.
*          The ADC is used to read the trimpot value and send it to the queue.
******************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "stm32f4xx_hal.h"
#include "processor_hal.h"
#include "board.h"
#include "mfs_trimpot.h"
#include <stdlib.h>

ADC_HandleTypeDef AdcHandle;
ADC_ChannelConfTypeDef AdcChanConfig;

QueueHandle_t QueueTrimpot;

void reg_mfs_trimpot_init(void) {
    __GPIOA_CLK_ENABLE();

    GPIOA->MODER |= (0x03 << (3 * 2));            // Analog input mode
    GPIOA->OSPEEDR &= ~(0x03 << (3 * 2));
    GPIOA->OSPEEDR |= 0x02 << (3 * 2);            // Fast speed
    GPIOA->PUPDR &= ~(0x03 << (3 * 2));           // No pull-up/down

    __ADC1_CLK_ENABLE();

    AdcHandle.Instance = ADC1;
    AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;
    AdcHandle.Init.Resolution            = ADC_RESOLUTION12b;
    AdcHandle.Init.ScanConvMode          = DISABLE;
    AdcHandle.Init.ContinuousConvMode    = DISABLE;
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;
    AdcHandle.Init.NbrOfDiscConversion   = 0;
    AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
    AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    AdcHandle.Init.NbrOfConversion       = 1;
    AdcHandle.Init.DMAContinuousRequests = DISABLE;
    AdcHandle.Init.EOCSelection          = DISABLE;

    HAL_ADC_Init(&AdcHandle);

    AdcChanConfig.Channel = ADC_CHANNEL_3;
    AdcChanConfig.Rank = 1;
    AdcChanConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    AdcChanConfig.Offset = 0;

    HAL_ADC_ConfigChannel(&AdcHandle, &AdcChanConfig);
}

int mfs_trimpot_get(void) {
    HAL_ADC_Start(&AdcHandle);
    while (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK);
    return HAL_ADC_GetValue(&AdcHandle);
}

void TaskTrimpot(void) {
    reg_mfs_trimpot_init();

    int last_adc = -1;

    for (;;) {
        int adc_value = mfs_trimpot_get();

        if (abs(adc_value - last_adc) > 10) {
            xQueueSend(QueueTrimpot, &adc_value, 0);
            last_adc = adc_value;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void TaskTrimpotInit(void) {
    QueueTrimpot = xQueueCreate(4, sizeof(int));
    xTaskCreate((void*)&TaskTrimpot, "TrimpotTask", 256, NULL, 2, NULL);
}
