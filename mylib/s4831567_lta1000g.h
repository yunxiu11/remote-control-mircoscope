 /** 
 **************************************************************
 * File: mylib/s4831567_lta1000g.c
 * Author: Yuxuan Zhou - s4831567
 * Date: 14/03/2025
 * Brief: mylib template driver
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4831567_reg_lta1000g_init () - Initialise LED Bar GPIO pins as outputs
 * ta1000g_seg_set() - Set the value (0 or 1) for the corresponding LED Bar segment GPIO pin.
 * s4831567_reg_lta1000g_write () -Write the LED Bar segments high or low.
 * s4831567_reg_lta1000g_brotate () -Perform a left or right bitwise rotate on the value (using only the 10 lower bits).
 *************************************************************** 
 */

#ifndef S4831567_ITA1000G_H
#define S4831567_ITA1000G_H

void s4831567_reg_lta1000g_init ();
void lta1000g_seg_set (int segment, unsigned char value);
void s4831567_reg_lta1000g_write (unsigned short value);
unsigned short s4831567_reg_lta1000g_brotate (unsigned short value, int direction);

#endif
