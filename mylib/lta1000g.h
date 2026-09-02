 /** 
 **************************************************************
 * File: mylib/lta1000g.c
 * Author: Yuxuan Zhou
 * Date: 14/03/2025
 * Brief: mylib template driver
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * reg_lta1000g_init () - Initialise LED Bar GPIO pins as outputs
 * ta1000g_seg_set() - Set the value (0 or 1) for the corresponding LED Bar segment GPIO pin.
 * reg_lta1000g_write () -Write the LED Bar segments high or low.
 * reg_lta1000g_brotate () -Perform a left or right bitwise rotate on the value (using only the 10 lower bits).
 *************************************************************** 
 */

#ifndef ITA1000G_H
#define ITA1000G_H

void reg_lta1000g_init ();
void lta1000g_seg_set (int segment, unsigned char value);
void reg_lta1000g_write (unsigned short value);
unsigned short reg_lta1000g_brotate (unsigned short value, int direction);

#endif
