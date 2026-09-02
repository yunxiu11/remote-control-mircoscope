/**
**************************************************************
* File: mylib/s4831567_mfs_pb.c
* Author: Yuxuan Zhou - s4831567
* Date: 14/03/2025
* Brief: mylib template driver
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* s4831567_reg_mfs_pb_init () - Enabled the specified pushbutton source, e.g. enables GPIO input and interrupt.
* s4831567_reg_mfs_pb_isr () - MFS Pushbutton Interrupt service routine.
* s4831567_reg_mfs_pb_press_get() - MFS Pushbutton Interrupt service routine.
* s4831567_reg_mfs_pb_press_reset() -Reset the MFS pushbutton event counter value to 0, for the specified pushbutton.
* s4831567TaskMfsPbInit() - MFS Pushbutton task initialisation function.
***************************************************************
*/

#ifndef S4831567_MFS_PB_H
#define S4831567_MFS_PB_H

#include "FreeRTOS.h"
#include "semphr.h"

static int MfPbPressCounter[3];
extern SemaphoreHandle_t s4831567SemaphoreS1;
extern SemaphoreHandle_t s4831567SemaphoreS3;
extern SemaphoreHandle_t onboardBtnSemaphore;

void s4831567_reg_mfs_pb_init(int pbSelect);
void s4831567_reg_mfs_pb_isr(int pbSelect);
int s4831567_reg_mfs_pb_press_get(int pbSelect);
void s4831567_reg_mfs_pb_press_reset(int pbSelect);
void s4831567TaskMfsPbInit(void);

#endif
