/*
 * 02_bit_masking.c - Bit Manipulation
 * Module 03: Arithmetic & Logic
 *
 * Description: Set, clear, toggle specific bits
 * Hardware: 8 LEDs on P1
 */

#include <8052.h>

void delay(void)
{
    unsigned int i, j;
    for (i = 0; i < 200; i++)
        for (j = 0; j < 500; j++);
}

void main(void)
{
    unsigned char value = 0x00;

    while (1) {
        /* Set bits 0, 2, 4, 6 (0x55) */
        value = 0x00;
        value |= (1 << 0);  /* Set bit 0 */
        value |= (1 << 2);  /* Set bit 2 */
        value |= (1 << 4);  /* Set bit 4 */
        value |= (1 << 6);  /* Set bit 6 */
        P1 = ~value;
        delay();

        /* Clear bits 0 and 2 */
        value &= ~(1 << 0);
        value &= ~(1 << 2);
        P1 = ~value;
        delay();

        /* Toggle all bits */
        value ^= 0xFF;
        P1 = ~value;
        delay();

        /* Set lower nibble */
        value = 0x0F;
        P1 = ~value;
        delay();

        /* Set upper nibble */
        value = 0xF0;
        P1 = ~value;
        delay();
    }
}
