#ifndef __S4831567_TXRADIO_H
#define __S4831567_TXRADIO_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include <stdint.h>

typedef enum
{
    CMD_XYZ = 0x22,
    CMD_ZOOM = 0x26,
    CMD_BRIGHT = 0x27,
    CMD_ORG = 0x0F,
    CMD_NULL = 0x00,
} command_opcode_t;

#define CMD_OPCODE_IDLE   0x01
#define CMD_OPCODE_X      0x02
#define CMD_OPCODE_Y      0x03
#define CMD_OPCODE_Z      0x04
#define CMD_OPCODE_ZOOM   0x05
#define CMD_OPCODE_BRIGHT 0x06
#define CMD_OPCODE_NEW    0x07
#define CMD_OPCODE_DEL    0x08
#define CMD_OPCODE_SYS    0x09
#define CMD_OPCODE_ORG    0x0F

void s4831567_txradio_init(void);
void s4831567_nrf24l01plus_init();
void join_task(void);
void cmdin_task(void);
void ledbar_task(void);
void cmdselect_task(void);
void segdisplay_task(void);
void trimpot_task(void);
void s4831567_txradio_send_packet(command_opcode_t cmd_type, uint8_t opcode, int val);

#endif