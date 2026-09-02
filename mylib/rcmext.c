/******************************************************************************
 * @file    rcmsys.c
 * @author  Yuxuan Zhou
 * @date    20/05/2025
 * @brief   RCMEXT task
 * @details EXTERNAL FUNCTIONS
 *          rcmext_init() - Initialise RCMEXT task
 *          rcmext_handle_command() - Handle RCMEXT command
 *          rcmext_task() - RCMEXT task
 ******************************************************************************/
#include "rcmext.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "debug_log.h"
#include "semphr.h"
#include <string.h>
#include <stdio.h>

#define EVTBIT_NEW (1 << 0)
#define EVTBIT_DEL (1 << 1)
#define EVTBIT_SYS (1 << 2)

#define CMD_OPCODE_NEW 0x07
#define CMD_OPCODE_DEL 0x08
#define CMD_OPCODE_SYS 0x09

static EventGroupHandle_t xExtEventGroup;
static TaskHandle_t txTaskHandle = NULL;


static void tx_radio_task(void)
{
    for (;;)
    {
        debug_log("TX Radio Task Running...\r\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void ext_control_task(void)
{
    EventBits_t bits;
    for (;;)
    {
        bits = xEventGroupWaitBits(
            xExtEventGroup,
            EVTBIT_NEW | EVTBIT_DEL | EVTBIT_SYS,
            pdTRUE,  // clear bits on exit
            pdFALSE, // wait for any bit
            portMAX_DELAY);

        if (bits & EVTBIT_NEW)
        {
            if (txTaskHandle == NULL)
            {
                BaseType_t created = xTaskCreate((void*)&tx_radio_task, "TXRAD", 512, NULL, 2, &txTaskHandle);
                if (created == pdPASS)
                {
                    debug_log("Create new radio task.\r\n");
                }
                else
                {
                    debug_log("Fail to create new radio task.\r\n");
                }
            }
            else
            {
                debug_log("TX Task already exists.\r\n");
            }
        }

        if (bits & EVTBIT_DEL)
        {
            if (txTaskHandle != NULL)
            {
                vTaskDelete(txTaskHandle);
                txTaskHandle = NULL;
                debug_log("TX Task deleted.\r\n");
            }
            else
            {
                debug_log("No TX Task to delete.\r\n");
            }
        }

        if (bits & EVTBIT_SYS)
        {
            char taskListBuffer[512];
            strcpy(taskListBuffer, "TaskName\tState\tPrio\tStack\tTaskNum\r\n");
            vTaskList(taskListBuffer + strlen(taskListBuffer));
            debug_log("Current Task List:\r\n");
            debug_log(taskListBuffer);
        }
    }
}

void rcmext_handle_command(uint8_t opcode)
{
    switch (opcode)
    {
    case CMD_OPCODE_NEW:
        xEventGroupSetBits(xExtEventGroup, EVTBIT_NEW);
        break;
    case CMD_OPCODE_DEL:
        xEventGroupSetBits(xExtEventGroup, EVTBIT_DEL);
        break;
    case CMD_OPCODE_SYS:
        xEventGroupSetBits(xExtEventGroup, EVTBIT_SYS);
        break;
    default:
        debug_log("Unknown Extended Command.\r\n");
        break;
    }
}

void rcmext_init(void)
{
    xExtEventGroup = xEventGroupCreate();
    if (xExtEventGroup == NULL)
    {
        debug_log("Failed to create Event Group.\r\n");
    }
    else
    {
        xTaskCreate((void*)&ext_control_task, "RCMEXT", 1024, NULL, 2, NULL);
    }
}
