/******************************************************************************
* @file    s4831567_rcmsys.c
* @author  Yuxuan Zhou
* @date    19/05/2025
* @brief   RCMSYS task
* @details This file contains the implementation of the RCMSYS task.
*          It handles the communication with the MFS and manages the LED states.
******************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "stm32f4xx_hal.h"
#include "processor_hal.h"
#include "board.h"
#include "s4831567_mfs_pb.h"
#include "s4831567_mfs_led.h"
#include "s4831567_switchbank.h"
#include "s4831567_lta1000g.h"
#include "s4831567_rcmsys.h"
#include "s4831567_hamming.h"
#include "nrf24l01plus.h"
#include <string.h>
#include "s4831567_seven_segment.h"

static int d2_on = 0;

void rcmsys_task(void)
{
    uint8_t raw_pkt[16] = {0};
    uint8_t encoded_pkt[32];

    for (;;) {
        if (xSemaphoreTake(s4831567SemaphoreS1, 0) == pdPASS ||
            xSemaphoreTake(s4831567SemaphoreS3, 0) == pdPASS) {

            d2_on = !d2_on;
            if (d2_on) {
                S4831567_REG_MFS_LED_D2_ON();
            } else {
                S4831567_REG_MFS_LED_D2_OFF();
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}


void s4831567_rcmsys_init(void)
{
    s4831567TaskMfsPbInit();
    s4831567_reg_mfs_led_init();
    s4831567_reg_lta1000g_init();
    s4831567_segdisplay_init();
    nrf24l01plus_init();

    xTaskCreate((void*)&rcmsys_task, "RCMSYS", 1024, NULL, 2, NULL);
}
