/**
**************************************************************
* File: mylib/mfs_trimpot.c
* Author: Yuxuan Zhou
* Date: 28/03/2025
* Brief: mylib template driver
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* reg_mfs_trimpot_init() - Initialise GPIO pins and ADC.
* mfs_trimpot_get() -  Internal Function to read the current Trimpot input value.
* reg_mfs_trimpot_getrev() -  Return the number of complete revolutions that the Trimpot
* is set to.
***************************************************************
*/
#ifndef MFS_TRIMPOT_H
#define MFS_TRIMPOT_H

void reg_mfs_trimpot_init(void);
int mfs_trimpot_get(void);
int mfs_trimpot_get(void);
void TaskTrimpot(void);
void TaskTrimpotInit(void);

#endif