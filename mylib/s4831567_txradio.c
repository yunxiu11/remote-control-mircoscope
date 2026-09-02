/******************************************************************************
 * @file    s4831567_txradio.c
 * @author  Yuxuan Zhou
 * @date    20/05/2025
 * @brief   TX radio task
 * @details EXTERNAL FUNCTIONS
            s4831567_txradio_init() - Initialise TX radio task
            s4831567_nrf24l01plus_init() - Initialise NRF24L01+ radio module
            join_task() - Task for handling the onboard "JOIN" button
            cmdselect_task() - Task for command selection
            cmdin_task() - Task for command input
            ledbar_task() - Task for LED bar
            switchbank_monitor_task() - Task to monitor switchbank
            segdisplay_task() - Task for seven segment display
            trimpot_task() - Task for trimpot
            s4831567_txradio_send_packet() - Send packet to TX radio
 ******************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "stm32f4xx_hal.h"
#include "processor_hal.h"
#include "board.h"
#include "s4831567_mfs_pb.h"
#include "s4831567_mfs_led.h"
#include "s4831567_rgb.h"
#include "s4831567_switchbank.h"
#include "s4831567_hamming.h"
#include "s4831567_lta1000g.h"
#include "s4831567_mfs_trimpot.h"
#include "nrf24l01plus.h"
#include "s4831567_rcmsys.h"
#include "s4831567_txradio.h"
#include "myconfig.h"
#include "debug_log.h"
#include "s4831567_seven_segment.h"
#include "s4831567_rcmext.h"
#include "s4831567_cli.h"
#include <string.h>
#include <stdlib.h>

static uint8_t current_opcode = CMD_OPCODE_IDLE;
static command_opcode_t current_cmd = CMD_XYZ;
static int d1_on = 0, d2_on = 0;
static int confirmed_display_val = 0;

int zoom_value = 100;
int bright_value = 0;

volatile uint8_t current_switch_val = 0;
uint8_t last_x_val = 0, last_y_val = 0;

extern QueueHandle_t s4831567QueueTrimpot;

// Initialize the NRF24L01+ radio module
void s4831567_nrf24l01plus_init(void)
{
    nrf24l01plus_init();
    BRD_delayInit();

    NRF_CE_LOW();
    nrf24l01plus_wb(NRF24L01P_WRITE_REG | NRF24L01P_TX_ADDR, myradiotxaddr, 5);
    nrf24l01plus_wb(NRF24L01P_WRITE_REG | NRF24L01P_RX_ADDR_P0, myradiotxaddr, 5);

    nrf24l01plus_wr(NRF24L01P_EN_AA, 0x00);
    nrf24l01plus_wr(NRF24L01P_EN_RXADDR, 0x01);
    nrf24l01plus_wr(NRF24L01P_RX_PW_P0, NRF24L01P_TX_PLOAD_WIDTH);
    nrf24l01plus_wr(NRF24L01P_RF_CH, MYRADIOCHAN);
    nrf24l01plus_wr(NRF24L01P_RF_SETUP, 0x06);
    nrf24l01plus_wr(NRF24L01P_CONFIG, 0x02);
}

// Task for handling the onboard "JOIN" button, sends a JOIN packet
void join_task(void)
{
    uint8_t raw_pkt[16] = {0};
    uint8_t encoded_pkt[32];

    while (1)
    {
        // Wait for onboard button press
        if (xSemaphoreTake(onboardBtnSemaphore, portMAX_DELAY) == pdPASS)
        {
            // Construct JOIN packet
            raw_pkt[0] = 0x20;
            raw_pkt[1] = 0x48;
            raw_pkt[2] = 0x31;
            raw_pkt[3] = 0x56;
            raw_pkt[4] = 0x76;
            memset(&raw_pkt[5], 0, 11);
            memcpy(&raw_pkt[5], "JOIN", 4);

            // Hamming encode and send the packet
            s4831567_hamming_packet_encode(raw_pkt, encoded_pkt);
            nrf24l01plus_send(encoded_pkt);

            d2_on = !d2_on;
            d2_on ? S4831567_REG_MFS_LED_D2_ON() : S4831567_REG_MFS_LED_D2_OFF();

            d1_on = !d1_on;
            d1_on ? S4831567_REG_MFS_LED_D1_ON() : S4831567_REG_MFS_LED_D1_OFF();
        }
    }
}

// Task for command selection, cycles through commands on S1 press
void cmdselect_task(void)
{
    while (1)
    {
        // Wait for S1 button press
        if (xSemaphoreTake(s4831567SemaphoreS1, portMAX_DELAY) == pdPASS)
        {
            // Cycle through command opcodes and types
            switch (current_opcode)
            {
            case CMD_OPCODE_IDLE:
                current_opcode = CMD_OPCODE_X;
                current_cmd = CMD_XYZ;
                break;
            case CMD_OPCODE_X:
                current_opcode = CMD_OPCODE_Y;
                current_cmd = CMD_XYZ;
                break;
            case CMD_OPCODE_Y:
                current_opcode = CMD_OPCODE_Z;
                current_cmd = CMD_XYZ;
                break;
            case CMD_OPCODE_Z:
                current_opcode = CMD_OPCODE_ZOOM;
                current_cmd = CMD_ZOOM;
                break;
            case CMD_OPCODE_ZOOM:
                current_opcode = CMD_OPCODE_BRIGHT;
                current_cmd = CMD_BRIGHT;
                break;
            case CMD_OPCODE_BRIGHT:
                current_opcode = CMD_OPCODE_NEW;
                current_cmd = CMD_NULL;
                break;
            case CMD_OPCODE_NEW:
                current_opcode = CMD_OPCODE_DEL;
                current_cmd = CMD_NULL;
                break;
            case CMD_OPCODE_DEL:
                current_opcode = CMD_OPCODE_SYS;
                current_cmd = CMD_NULL;
                break;
            case CMD_OPCODE_SYS:
                current_opcode = CMD_OPCODE_ORG;
                current_cmd = CMD_ORG;
                break;

            case CMD_OPCODE_ORG:
                zoom_value = 100;
                bright_value = 0;
                current_opcode = CMD_OPCODE_IDLE;
                current_cmd = CMD_XYZ;
                break;
            default:
                current_opcode = CMD_OPCODE_IDLE;
                current_cmd = CMD_NULL;
                break;
            }

            if (current_opcode == CMD_OPCODE_NEW ||
                current_opcode == CMD_OPCODE_DEL ||
                current_opcode == CMD_OPCODE_SYS)
            {
                s4831567_rcmext_handle_command(current_opcode);
            }

            d2_on = !d2_on;
            d2_on ? S4831567_REG_MFS_LED_D2_ON() : S4831567_REG_MFS_LED_D2_OFF();
        }
    }
}

// Task for command input, sends command packet on S3 press
void cmdin_task(void)
{
    uint8_t raw_pkt[16];
    uint8_t encoded_pkt[32];

    while (1)
    {
        // Wait for S3 button press
        if (xSemaphoreTake(s4831567SemaphoreS3, portMAX_DELAY) == pdPASS)
        {

            raw_pkt[0] = current_cmd;
            raw_pkt[1] = 0x48;
            raw_pkt[2] = 0x31;
            raw_pkt[3] = 0x56;
            raw_pkt[4] = 0x76;
            memset(&raw_pkt[5], 0, 11);

            char payload[12] = {0};

            // Build payload based on command type
            switch (current_cmd)
            {
            case CMD_XYZ:
            {
                uint8_t x_val = 0, y_val = 0, z_val = 0;

                switch (current_opcode)
                {
                case CMD_OPCODE_X:
                    x_val = current_switch_val;
                    last_x_val = x_val;
                    y_val = 0;
                    z_val = 0;
                    confirmed_display_val = x_val;
                    break;
                case CMD_OPCODE_Y:
                    y_val = current_switch_val;
                    last_y_val = y_val;
                    x_val = last_x_val;
                    z_val = 0;
                    confirmed_display_val = y_val;
                    break;
                case CMD_OPCODE_Z:
                    z_val = (current_switch_val > 99) ? 99 : current_switch_val;
                    x_val = last_x_val;
                    y_val = last_y_val;
                    confirmed_display_val = z_val;
                    break;
                default:
                    break;
                }

                snprintf(payload, sizeof(payload), "XYZ%03d%03d%02d", x_val, y_val, z_val);
                break;
            }

            case CMD_ZOOM:
                snprintf(payload, sizeof(payload), "ZOOM%03d", zoom_value);
                confirmed_display_val = zoom_value;
                break;

            case CMD_BRIGHT:
                snprintf(payload, sizeof(payload), "BRIGHT%c%03d", bright_value >= 0 ? ' ' : '-', abs(bright_value));
                confirmed_display_val = bright_value;
                break;

            case CMD_ORG:
            {
                zoom_value = 100;
                bright_value = 0;
                last_x_val = 0;
                last_y_val = 0;
                confirmed_display_val = 0;

                s4831567_txradio_send_packet(CMD_ORG, CMD_OPCODE_ORG, 0);
                continue;
            }
            }

            memcpy(&raw_pkt[5], payload, strlen(payload));
            s4831567_hamming_packet_encode(raw_pkt, encoded_pkt);
            nrf24l01plus_send(encoded_pkt);

            // Display current switch value on seven segment
            s4831567_segdisplay_write_number(current_switch_val);

            d1_on = !d1_on;
            d1_on ? S4831567_REG_MFS_LED_D1_ON() : S4831567_REG_MFS_LED_D1_OFF();

            d2_on = !d2_on;
            d2_on ? S4831567_REG_MFS_LED_D2_ON() : S4831567_REG_MFS_LED_D2_OFF();
        }
    }
}

// Task for LED bar, displays current opcode
void ledbar_task(void)
{
    while (1)
    {
        s4831567_reg_lta1000g_write(current_opcode);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// Task to monitor switchbank and update current value
void switchbank_monitor_task(void)
{
    s4831567SwitchbankMsg sw_msg;
    while (1)
    {
        if (xQueueReceive(s4831567QueueSwitchbank, &sw_msg, portMAX_DELAY) == pdPASS)
        {
            current_switch_val = sw_msg.switch_val;
        }
    }
}

// Task for seven segment display, shows value based on current command
void segdisplay_task(void)
{
    while (1)
    {
        int display_val;

        switch (current_opcode)
        {
        case CMD_OPCODE_ZOOM:
            display_val = zoom_value;
            break;
        case CMD_OPCODE_BRIGHT:
            display_val = bright_value;
            break;
        case CMD_OPCODE_IDLE:
            display_val = 0;
            break;
        default:
            display_val = current_switch_val;
            break;
        }

        s4831567_segdisplay_write_number(confirmed_display_val);
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

// Task for RGB LED, changes color based on opcode
void rgb_task(void)
{
    static uint8_t last_rgb_mask = 0xFF;
    uint8_t new_rgb_mask = 0x00;
    s4831567_reg_rgb_colour_set(0x05);

    while (1)
    {
        if (current_opcode == CMD_OPCODE_IDLE)
        {
            new_rgb_mask = 0x05;
        }

        if (new_rgb_mask != last_rgb_mask)
        {
            s4831567_reg_rgb_colour_set(new_rgb_mask);
            last_rgb_mask = new_rgb_mask;
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// Task to monitor trimpot, adjusts zoom/brightness and controls RGB
void trimpot_task(void)
{
    int adc_val;
    static int last_level = -1;
    static uint8_t last_rgb_mask = 0xFF;

    while (1)
    {
        if (xQueueReceive(s4831567QueueTrimpot, &adc_val, 0) == pdPASS)
        {

            int level = adc_val / 409;

            if (level != last_level)
            {
                int diff = level - last_level;

                if (last_level == -1)
                {
                    last_level = level;
                    continue;
                }

                if (current_opcode == CMD_OPCODE_ZOOM)
                {
                    zoom_value += diff * 20;
                    if (zoom_value > 200)
                        zoom_value = 200;
                    if (zoom_value < 0)
                        zoom_value = 0;
                }

                if (current_opcode == CMD_OPCODE_BRIGHT)
                {
                    bright_value += diff * 20;
                    if (bright_value > 100)
                        bright_value = 100;
                    if (bright_value < -100)
                        bright_value = -100;
                }

                uint8_t new_rgb = 0x05;
                if (diff > 0)
                    new_rgb = 0x03;
                else if (diff < 0)
                    new_rgb = 0x06;

                if (new_rgb != last_rgb_mask)
                {
                    s4831567_reg_rgb_colour_set(new_rgb);
                    last_rgb_mask = new_rgb;
                }

                last_level = level;
            }
        }

        // Restore default RGB color if not in zoom/bright mode
        if (current_opcode != CMD_OPCODE_ZOOM && current_opcode != CMD_OPCODE_BRIGHT)
        {
            if (last_rgb_mask != 0x05)
            {
                s4831567_reg_rgb_colour_set(0x05);
                last_rgb_mask = 0x05;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void s4831567_txradio_send_packet(command_opcode_t cmd_type, uint8_t opcode, int val)
{
    s4831567_nrf24l01plus_init();
    uint8_t raw_pkt[16] = {0};
    uint8_t encoded_pkt[32];
    char payload[12] = {0};

    raw_pkt[0] = cmd_type;
    raw_pkt[1] = 0x48;
    raw_pkt[2] = 0x31;
    raw_pkt[3] = 0x56;
    raw_pkt[4] = 0x76;

    switch (cmd_type)
    {
    case CMD_XYZ:
        if (opcode == CMD_OPCODE_X)
            snprintf(payload, sizeof(payload), "XYZ%03d000000", val);
        else if (opcode == CMD_OPCODE_Y)
            snprintf(payload, sizeof(payload), "XYZ000%03d00", val);
        else if (opcode == CMD_OPCODE_Z)
            snprintf(payload, sizeof(payload), "XYZ000000%02d", val > 99 ? 99 : val);
        break;
    case CMD_ZOOM:
        snprintf(payload, sizeof(payload), "ZOOM%03d", val);
        break;
    case CMD_BRIGHT:
        snprintf(payload, sizeof(payload), "BRIGHT%c%03d", val >= 0 ? ' ' : '-', abs(val));
        break;
    case CMD_ORG:
        s4831567_txradio_send_packet(CMD_XYZ, CMD_OPCODE_X, 0);
        s4831567_txradio_send_packet(CMD_ZOOM, CMD_OPCODE_ZOOM, 100);
        s4831567_txradio_send_packet(CMD_BRIGHT, CMD_OPCODE_BRIGHT, 0);
        return;
    default:
        return;
    }

    memcpy(&raw_pkt[5], payload, strlen(payload));
    s4831567_hamming_packet_encode(raw_pkt, encoded_pkt);
    nrf24l01plus_send(encoded_pkt);
}

void s4831567_txradio_init(void)
{
    BRD_debuguart_init();
    s4831567_reg_lta1000g_init();
    s4831567_segdisplay_init();
    s4831567_reg_rgb_init();
    s4831567_nrf24l01plus_init();
    s4831567TaskMfsPbInit();
    s4831567TaskSwitchbankInit();
    s4831567TaskTrimpotInit();
    s4831567_rcmext_init();
    s4831567_cli_init();

    xTaskCreate((void *)&join_task, "JOIN", 1024, NULL, 2, NULL);
    xTaskCreate((void *)&cmdselect_task, "CMDSELECT", 1024, NULL, 2, NULL);
    xTaskCreate((void *)&cmdin_task, "CMDIN", 1024, NULL, 2, NULL);
    xTaskCreate((void *)&ledbar_task, "LEDBAR", 1024, NULL, 2, NULL);
    xTaskCreate((void *)&switchbank_monitor_task, "SWMON", 256, NULL, 1, NULL);
    xTaskCreate((void *)&segdisplay_task, "SEGDISP", 1024, NULL, 2, NULL);
    xTaskCreate((void *)&rgb_task, "RGB", 256, NULL, 1, NULL);
    xTaskCreate((void *)&trimpot_task, "TRIMPOT", 256, NULL, 1, NULL);
    xTaskCreate((void *)&s4831567_cli_task, "CLI", 1024, NULL, 2, NULL);
}
