 /** 
 **************************************************************
 * File: mylib/rgb.c
 * Author: Yuxuan Zhou
 * Date: 02/04/2025
 * Brief: mylib template driver
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * lib_hamming_encode() - Return the 16bit encoded value of a byte.
 * lib_hamming_decode() - Return the decoded half byte from a byte. Er
 * rors must be detected and corrected.
 * hamming_hbtye_encode() - Internal function to Encode a half byte.
 * lib_hamming_parity_error() -  Return 1 if a parity error has occurred, else 0.
 *************************************************************** 
 */

#ifndef HAMMING_H
#define HAMMING_H

int lib_hamming_parity_error(unsigned char value);
unsigned short lib_hamming_byte_encode(unsigned char value);
unsigned char lib_hamming_byte_decode(unsigned char value);
unsigned char hamming_hbyte_encode(unsigned char value);
void hamming_packet_encode(uint8_t *input16, uint8_t *output32);


#endif