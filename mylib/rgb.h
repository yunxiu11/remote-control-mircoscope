 /** 
 **************************************************************
 * File: mylib/rgb.c
 * Author: Yuxuan Zhou
 * Date: 28/03/2025
 * Brief: mylib template driver
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * reg_rgb_init() - Initialise GPIO pins and Timer.
 * reg_rgb_brightness_write() - Set the brightness of the RGB LED.
 * reg_rgb_brightness_read() - Read the current brightness of the RGB LED.
 * reg_rgb_colour_set() - Set the colour of the RGB LED.
 *************************************************************** 
 */

#ifndef RGB_H
#define RGB_H

void reg_rgb_init(void);

void reg_rgb_brightness_write(unsigned short level);

unsigned short reg_rgb_brightness_read(void);

void reg_rgb_colour_set(unsigned char rgb_mask);

#endif