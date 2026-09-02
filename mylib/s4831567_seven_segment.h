#ifndef __S4831567_SEGDISPLAY_H
#define __S4831567_SEGDISPLAY_H

#include <stdint.h>

void s4831567_segdisplay_init(void);
void s4831567_segdisplay_shiftout_dual(uint8_t seg_data, uint8_t digit_sel);
void s4831567_segdisplay_write_digit_at(uint8_t digit, uint8_t pos);
void s4831567_segdisplay_write_number(int number);

#endif
