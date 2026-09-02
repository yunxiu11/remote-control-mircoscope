/******************************************************************************
 * @file    s4831567_hamming.c
 * @author  Yuxuan Zhou
 * @date    16/05/2025
 * @brief   The Hamming LIB provides hamming byte encode, byte decode and half byte encode functions.
 ******************************************************************************/

#include "stm32f4xx_hal.h"
#include "processor_hal.h"
#include "board.h"
#include "s4831567_switchbank.h"
#include "debug_log.h"

int s4831567_lib_hamming_parity_error(unsigned char value);
unsigned short s4831567_lib_hamming_byte_encode(unsigned char value);
unsigned char s4831567_lib_hamming_byte_decode(unsigned char value);
unsigned char hamming_hbyte_encode(unsigned char value);

unsigned char s4831567_lib_hamming_byte_decode(unsigned char value)
{
    int err = s4831567_lib_hamming_parity_error(value);

    if (err == 1)
    {
        unsigned char d0 = !!(value & (1 << 4));
        unsigned char d1 = !!(value & (1 << 5));
        unsigned char d2 = !!(value & (1 << 6));
        unsigned char d3 = !!(value & (1 << 7));
        unsigned char h0 = !!(value & (1 << 1));
        unsigned char h1 = !!(value & (1 << 2));
        unsigned char h2 = !!(value & (1 << 3));
        unsigned char syndrome = 0;

        unsigned char c0 = d1 ^ d2 ^ d3;
        unsigned char c1 = d0 ^ d2 ^ d3;
        unsigned char c2 = d0 ^ d1 ^ d3;

        if (c0 != h0)
            syndrome |= 0x01;
        if (c1 != h1)
            syndrome |= 0x02;
        if (c2 != h2)
            syndrome |= 0x04;

        if (syndrome == 0)
        {
            value ^= (1 << 0);
        }
        else if (syndrome <= 7)
        {
            value ^= (1 << syndrome);
        }
    }
    else if (err == 2)
    {
        return 0xFF; // 2-bit error
    }

    unsigned char decoded = ((!!(value & (1 << 7))) << 3) |
                            ((!!(value & (1 << 6))) << 2) |
                            ((!!(value & (1 << 5))) << 1) |
                            ((!!(value & (1 << 4))) << 0);

    return decoded;
}

unsigned char hamming_hbyte_encode(unsigned char value)
{
    unsigned char d0, d1, d2, d3;
    unsigned char p0 = 0, h0, h1, h2;
    unsigned char i;
    unsigned char out;

    /* extract bits */
    d0 = !!(value & 0x1);
    d1 = !!(value & 0x2);
    d2 = !!(value & 0x4);
    d3 = !!(value & 0x8);

    /* calculate hamming parity bits */
    h0 = d1 ^ d2 ^ d3;
    h1 = d0 ^ d2 ^ d3;
    h2 = d0 ^ d1 ^ d3;

    /* generate out byte without parity bit P0 */
    out = (h0 << 1) | (h1 << 2) | (h2 << 3) |
          (d0 << 4) | (d1 << 5) | (d2 << 6) | (d3 << 7);

    /* calculate even parity bit */
    for (i = 1; i < 8; i++)
        p0 = p0 ^ !!(out & (1 << i));

    out |= p0;

    return (out);
}

unsigned short s4831567_lib_hamming_byte_encode(unsigned char value)
{
    uint16_t out;

    /* first encode D0..D3 (first 4 bits),
     * then D4..D7 (second 4 bits).
     */
    out = hamming_hbyte_encode(value & 0xF) |
          (hamming_hbyte_encode(value >> 4) << 8);

    return (out);
}

int s4831567_lib_hamming_parity_error(unsigned char value)
{
    unsigned char d0, d1, d2, d3;
    unsigned char h0, h1, h2;
    unsigned char c0, c1, c2;
    unsigned char syndrome = 0;
    unsigned char parity = 0;
    int i;

    d0 = !!(value & (1 << 4));
    d1 = !!(value & (1 << 5));
    d2 = !!(value & (1 << 6));
    d3 = !!(value & (1 << 7));

    h0 = !!(value & (1 << 1));
    h1 = !!(value & (1 << 2));
    h2 = !!(value & (1 << 3));

    c0 = d1 ^ d2 ^ d3;
    c1 = d0 ^ d2 ^ d3;
    c2 = d0 ^ d1 ^ d3;

    if (c0 != h0)
        syndrome |= 0x01;
    if (c1 != h1)
        syndrome |= 0x02;
    if (c2 != h2)
        syndrome |= 0x04;

    for (i = 1; i < 8; i++)
    {
        parity ^= !!(value & (1 << i));
    }
    int parity_error = (parity != (value & 0x01));

    if (syndrome == 0 && !parity_error)
        return 0;
    else if ((syndrome != 0 && parity_error) || (syndrome == 0 && parity_error))
        return 1;
    else
        return 2;
}

void s4831567_hamming_packet_encode(uint8_t *input16, uint8_t *output32)
{
    for (int i = 0; i < 16; i++)
    {
        uint8_t byte = input16[i];
        output32[2 * i] = hamming_hbyte_encode(byte & 0x0F);            // low nibble
        output32[2 * i + 1] = hamming_hbyte_encode((byte >> 4) & 0x0F); // high nibble
    }
}
