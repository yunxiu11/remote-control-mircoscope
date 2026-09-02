/**
 ******************************************************************************
 * @file    s4831567_cli.c
 * @author  Yuxuan Zhou
 * @date    22/05/2025
 * @brief   FreeRTOS CLI commands for DT1–DT5 (e.g. x, y, z, zoom, bright, org)
 * @details external functions
 *          s4831567_cli_init() - Initialise CLI commands
 *          s4831567_cli_task() - CLI task
 ******************************************************************************
 */

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS_CLI.h"
#include "debug_log.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "s4831567_txradio.h"
#include "s4831567_hamming.h"
#include "nrf24l01plus.h"

#define CLI_INPUT_BUFFER_LEN 100

static char cInputString[CLI_INPUT_BUFFER_LEN];
static char *pcOutputString;
static int InputIndex = 0;

static int cli_x_val = 0;
static int cli_y_val = 0;
static int cli_z_val = 0;

// Function to initialise CLI commands
static BaseType_t parseSingleIntParam(const char *pcCommandString, int *outVal)
{
    long param_len;
    const char *param = FreeRTOS_CLIGetParameter(pcCommandString, 1, &param_len);
    if (param != NULL)
    {
        char numbuf[8] = {0};
        strncpy(numbuf, param, param_len);
        *outVal = atoi(numbuf);
        return pdTRUE;
    }
    return pdFALSE;
}

// Command handlers for CLI commands
static BaseType_t prvXCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    int val;
    if (parseSingleIntParam(pcCommandString, &val)) {
        cli_x_val = val;

        char payload[12];
        snprintf(payload, sizeof(payload), "XYZ%03d%03d%02d", cli_x_val, cli_y_val, cli_z_val);

        uint8_t raw_pkt[16] = {0x22, 0x48, 0x31, 0x56, 0x76};
        uint8_t encoded_pkt[32];
        memcpy(&raw_pkt[5], payload, strlen(payload));
        s4831567_hamming_packet_encode(raw_pkt, encoded_pkt);
        nrf24l01plus_send(encoded_pkt);

        debug_log("[CLI] Sent X = %d\r\n", cli_x_val);
    } else {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Invalid X command\r\n");
    }
    return pdFALSE;
}

// Command handlers for CLI commands
static BaseType_t prvYCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    int val;
    if (parseSingleIntParam(pcCommandString, &val)) {
        cli_y_val = val;

        char payload[12];
        snprintf(payload, sizeof(payload), "XYZ%03d%03d%02d", cli_x_val, cli_y_val, cli_z_val);

        uint8_t raw_pkt[16] = {0x22, 0x48, 0x31, 0x56, 0x76};
        uint8_t encoded_pkt[32];
        memcpy(&raw_pkt[5], payload, strlen(payload));
        s4831567_hamming_packet_encode(raw_pkt, encoded_pkt);
        nrf24l01plus_send(encoded_pkt);

        debug_log("[CLI] Sent Y = %d\r\n", cli_y_val);
    } else {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Invalid Y command\r\n");
    }
    return pdFALSE;
}

// Command handlers for CLI commands
static BaseType_t prvZCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    int val;
    if (parseSingleIntParam(pcCommandString, &val)) {
        cli_z_val = (val > 99) ? 99 : val;

        char payload[12];
        snprintf(payload, sizeof(payload), "XYZ%03d%03d%02d", cli_x_val, cli_y_val, cli_z_val);

        uint8_t raw_pkt[16] = {0x22, 0x48, 0x31, 0x56, 0x76};
        uint8_t encoded_pkt[32];
        memcpy(&raw_pkt[5], payload, strlen(payload));
        s4831567_hamming_packet_encode(raw_pkt, encoded_pkt);
        nrf24l01plus_send(encoded_pkt);

        debug_log("[CLI] Sent Z = %d\r\n", cli_z_val);
    } else {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Invalid Z command\r\n");
    }
    return pdFALSE;
}

// Command handlers for CLI commands
static BaseType_t prvZoomCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    int val;
    if (parseSingleIntParam(pcCommandString, &val))
    {
        s4831567_txradio_send_packet(CMD_ZOOM, CMD_OPCODE_ZOOM, val);
        debug_log("[CLI] Set Zoom = %d\r\n", val);
    }
    else
    {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Invalid Zoom command\r\n");
    }
    return pdFALSE;
}

// Command handlers for CLI commands
static BaseType_t prvBrightCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    int val;
    if (parseSingleIntParam(pcCommandString, &val))
    {
        s4831567_txradio_send_packet(CMD_BRIGHT, CMD_OPCODE_BRIGHT, val);
        debug_log("[CLI] Set Bright = %d\r\n", val);
    }
    else
    {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Invalid Bright command\r\n");
    }
    return pdFALSE;
}

// Command handlers for CLI commands
static BaseType_t prvOrgCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    cli_x_val = 0;
    cli_y_val = 0;
    cli_z_val = 0;

    debug_log("[CLI] Resetting to origin + defaults\r\n");
    s4831567_txradio_send_packet(CMD_ORG, CMD_OPCODE_ORG, 0);
    return pdFALSE;
}

// Function to initialise CLI commands
void s4831567_cli_init(void)
{
    static const CLI_Command_Definition_t x_cmd = {"x", "x <value>:\tSet X value\r\n", prvXCommand, 1};
    static const CLI_Command_Definition_t y_cmd = {"y", "y <value>:\tSet Y value\r\n", prvYCommand, 1};
    static const CLI_Command_Definition_t z_cmd = {"z", "z <value>:\tSet Z value\r\n", prvZCommand, 1};
    static const CLI_Command_Definition_t zoom_cmd = {"zoom", "zoom <value>:\tSet zoom value\r\n", prvZoomCommand, 1};
    static const CLI_Command_Definition_t bright_cmd = {"bright", "bright <value>:\tSet brightness\r\n", prvBrightCommand, 1};
    static const CLI_Command_Definition_t org_cmd = {"org", "org:\tReset to origin\r\n", prvOrgCommand, 0};

    FreeRTOS_CLIRegisterCommand(&x_cmd);
    FreeRTOS_CLIRegisterCommand(&y_cmd);
    FreeRTOS_CLIRegisterCommand(&z_cmd);
    FreeRTOS_CLIRegisterCommand(&zoom_cmd);
    FreeRTOS_CLIRegisterCommand(&bright_cmd);
    FreeRTOS_CLIRegisterCommand(&org_cmd);
}

// Function to handle CLI input and process commands
void s4831567_cli_task(void)
{
    char cRxedChar;

    memset(cInputString, 0, sizeof(cInputString));
    pcOutputString = FreeRTOS_CLIGetOutputBuffer();

    for (;;)
    {
        cRxedChar = debug_getc();

        if (cRxedChar != '\0')
        {
            debug_putc(cRxedChar);

            if (cRxedChar == '\r' || cRxedChar == '\n')
            {
                debug_putc('\n');
                debug_flush();

                cInputString[InputIndex] = '\0';
                BaseType_t xReturned = FreeRTOS_CLIProcessCommand(cInputString, pcOutputString, configCOMMAND_INT_MAX_OUTPUT_SIZE);

                if (strlen(pcOutputString) == 0)
                {
                    debug_log("Command not recognised. Enter 'help' to view a list of available commands.\r\n");
                }
                else
                {
                    while (xReturned != pdFALSE)
                    {
                        debug_log(pcOutputString);
                        xReturned = FreeRTOS_CLIProcessCommand(NULL, pcOutputString, configCOMMAND_INT_MAX_OUTPUT_SIZE);
                    }
                }

                memset(cInputString, 0, sizeof(cInputString));
                InputIndex = 0;
            }
            else if (cRxedChar == '\b')
            {
                if (InputIndex > 0)
                {
                    InputIndex--;
                    cInputString[InputIndex] = '\0';
                }
            }
            else if (InputIndex < CLI_INPUT_BUFFER_LEN - 1)
            {
                cInputString[InputIndex++] = cRxedChar;
            }
        }

        vTaskDelay(100);
    }
}
