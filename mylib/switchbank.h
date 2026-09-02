/** 
 **************************************************************
 * File: mylib/switchbank.c
 * Author: Yuxuan Zhou
 * Date: 02/04/2025
 * Brief: mylib template driver
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 * reg_switchbank_init() -  Enabled the switch bank GPIO pins as inputs.
 * reg_switchbank_read() - Returns a bit mask of the switch bank inputs.
 * TaskSwitchbank() - Switch bank task initialisation function.
 * TaskSwitchbankInit() - Switch bank task initialisation function.
 ***************************************************************
 * 
 *************************************************************** 
 */

 #ifndef SWITCHBANK_H
 #define SWITCHBANK_H
 
 #include "FreeRTOS.h"
 #include "queue.h"
 
 typedef struct {
     uint8_t switch_val;
 } SwitchbankMsg;
 
 extern QueueHandle_t QueueSwitchbank;
 
 void reg_switchbank_init();
 unsigned char reg_switchbank_read();
 void TaskSwitchbank(void);
 void TaskSwitchbankInit(void);
 
 #endif