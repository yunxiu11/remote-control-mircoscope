/** 
 **************************************************************
 * File: mylib/s4831567_switchbank.c
 * Author: Yuxuan Zhou - s4831567
 * Date: 02/04/2025
 * Brief: mylib template driver
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 * s4831567_reg_switchbank_init() -  Enabled the switch bank GPIO pins as inputs.
 * s4831567_reg_switchbank_read() - Returns a bit mask of the switch bank inputs.
 * s4831567TaskSwitchbank() - Switch bank task initialisation function.
 * s4831567TaskSwitchbankInit() - Switch bank task initialisation function.
 ***************************************************************
 * 
 *************************************************************** 
 */

 #ifndef S4831567_SWITCHBANK_H
 #define S4831567_SWITCHBANK_H
 
 #include "FreeRTOS.h"
 #include "queue.h"
 
 typedef struct {
     uint8_t switch_val;
 } s4831567SwitchbankMsg;
 
 extern QueueHandle_t s4831567QueueSwitchbank;
 
 void s4831567_reg_switchbank_init();
 unsigned char s4831567_reg_switchbank_read();
 void s4831567TaskSwitchbank(void);
 void s4831567TaskSwitchbankInit(void);
 
 #endif