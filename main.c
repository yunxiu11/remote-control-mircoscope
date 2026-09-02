#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stm32f4xx_hal.h"
#include "processor_hal.h"
#include "board.h"
#include "nrf24l01plus.h"
#include "debug_log.h"
#include "s4831567_mfs_pb.h"
#include "s4831567_rgb.h"
#include "s4831567_mfs_led.h"
#include "s4831567_rcmsys.h"
#include "s4831567_txradio.h"
#include "s4831567_seven_segment.h"
#include "s4831567_switchbank.h"
#include "s4831567_cli.h"
#include <string.h>

int main(void)
{
    HAL_Init();
    taskENTER_CRITICAL();
    BRD_sysmon_init();
    BRD_LEDInit();
    BRD_debuguart_init();
    taskEXIT_CRITICAL();

    s4831567_rcmsys_init();
    s4831567_txradio_init();
    
    vTaskStartScheduler();

    while (1)
    {

    }
}
