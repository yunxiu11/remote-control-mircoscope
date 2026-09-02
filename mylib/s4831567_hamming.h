 /** 
 **************************************************************
 * File: mylib/s4831567_rgb.c
 * Author: Yuxuan Zhou - s4831567
 * Date: 02/04/2025
 * Brief: mylib template driver
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4831567_lib_hamming_encode() - Return the 16bit encoded value of a byte.
 * s4831567_lib_hamming_decode() - Return the decoded half byte from a byte. Er
 * rors must be detected and corrected.
 * hamming_hbtye_encode() - Internal function to Encode a half byte.
 * s4831567_lib_hamming_parity_error() -  Return 1 if a parity error has occurred, else 0.
 *************************************************************** 
 */

#ifndef S4831567_HAMMING_H
#define S4831567_HAMMING_H

int s4831567_lib_hamming_parity_error(unsigned char value);
unsigned short s4831567_lib_hamming_byte_encode(unsigned char value);
unsigned char s4831567_lib_hamming_byte_decode(unsigned char value);
unsigned char hamming_hbyte_encode(unsigned char value);
void s4831567_hamming_packet_encode(uint8_t *input16, uint8_t *output32);


#endif