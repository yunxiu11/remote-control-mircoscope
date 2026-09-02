#ifndef __SEGDISPLAY_H
#define __SEGDISPLAY_H

#include <stdint.h>

void segdisplay_init(void);
void segdisplay_shiftout_dual(uint8_t seg_data, uint8_t digit_sel);
void segdisplay_write_digit_at(uint8_t digit, uint8_t pos);
void segdisplay_write_number(int number);

#endif
