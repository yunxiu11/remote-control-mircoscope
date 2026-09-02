/**
**************************************************************
* File: mylib/s4831567_mfs_trimpot.c
* Author: Yuxuan Zhou - s4831567
* Date: 28/03/2025
* Brief: mylib template driver
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* s4831567_reg_mfs_trimpot_init() - Initialise GPIO pins and ADC.
* mfs_trimpot_get() -  Internal Function to read the current Trimpot input value.
* s4831567_reg_mfs_trimpot_getrev() -  Return the number of complete revolutions that the Trimpot
* is set to.
***************************************************************
*/
#ifndef S4831567_MFS_TRIMPOT_H
#define S4831567_MFS_TRIMPOT_H

void s4831567_reg_mfs_trimpot_init(void);
int mfs_trimpot_get(void);
int mfs_trimpot_get(void);
void s4831567TaskTrimpot(void);
void s4831567TaskTrimpotInit(void);

#endif