#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stm32f4xx_hal.h"
#include "processor_hal.h"
#include "board.h"
#include "nrf24l01plus.h"
#include "debug_log.h"
#include "mfs_pb.h"
#include "rgb.h"
#include "mfs_led.h"
#include "rcmsys.h"
#include "txradio.h"
#include "seven_segment.h"
#include "switchbank.h"
#include "cli.h"
#include <string.h>

int main(void)
{
    HAL_Init();
    taskENTER_CRITICAL();
    BRD_sysmon_init();
    BRD_LEDInit();
    BRD_debuguart_init();
    taskEXIT_CRITICAL();

    rcmsys_init();
    txradio_init();
    
    vTaskStartScheduler();

    while (1)
    {

    }
}
