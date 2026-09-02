 /** 
 **************************************************************
 * File: mylib/s4831567_rgb.c
 * Author: Yuxuan Zhou - s4831567
 * Date: 28/03/2025
 * Brief: mylib template driver
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4831567_reg_rgb_init() - Initialise GPIO pins and Timer.
 * s4831567_reg_rgb_brightness_write() - Set the brightness of the RGB LED.
 * s4831567_reg_rgb_brightness_read() - Read the current brightness of the RGB LED.
 * s4831567_reg_rgb_colour_set() - Set the colour of the RGB LED.
 *************************************************************** 
 */

#ifndef S4831567_RGB_H
#define S4831567_RGB_H

void s4831567_reg_rgb_init(void);

void s4831567_reg_rgb_brightness_write(unsigned short level);

unsigned short s4831567_reg_rgb_brightness_read(void);

void s4831567_reg_rgb_colour_set(unsigned char rgb_mask);

#endif