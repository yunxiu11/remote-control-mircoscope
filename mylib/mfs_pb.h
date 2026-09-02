/**
**************************************************************
* File: mylib/mfs_pb.c
* Author: Yuxuan Zhou
* Date: 14/03/2025
* Brief: mylib template driver
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* reg_mfs_pb_init () - Enabled the specified pushbutton source, e.g. enables GPIO input and interrupt.
* reg_mfs_pb_isr () - MFS Pushbutton Interrupt service routine.
* reg_mfs_pb_press_get() - MFS Pushbutton Interrupt service routine.
* reg_mfs_pb_press_reset() -Reset the MFS pushbutton event counter value to 0, for the specified pushbutton.
* TaskMfsPbInit() - MFS Pushbutton task initialisation function.
***************************************************************
*/

#ifndef MFS_PB_H
#define MFS_PB_H

#include "FreeRTOS.h"
#include "semphr.h"

static int MfPbPressCounter[3];
extern SemaphoreHandle_t SemaphoreS1;
extern SemaphoreHandle_t SemaphoreS3;
extern SemaphoreHandle_t onboardBtnSemaphore;

void reg_mfs_pb_init(int pbSelect);
void reg_mfs_pb_isr(int pbSelect);
int reg_mfs_pb_press_get(int pbSelect);
void reg_mfs_pb_press_reset(int pbSelect);
void TaskMfsPbInit(void);

#endif
