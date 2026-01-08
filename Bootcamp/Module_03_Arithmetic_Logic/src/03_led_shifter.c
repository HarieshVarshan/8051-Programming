/*
 * 03_led_shifter.c - LED Shift Operations
 * Module 03: Arithmetic & Logic
 *
 * Description: Demonstrates left/right shift
 * Hardware: 8 LEDs on P1
 */

#include <8052.h>

void delay(void)
{
    unsigned int i, j;
    for (i = 0; i < 100; i++)
        for (j = 0; j < 500; j++);
}

void main(void)
{
    unsigned char i;
    unsigned char pattern;

    while (1) {
        /* Left shift: 1 LED moves left */
        pattern = 0x01;
        for (i = 0; i < 8; i++) {
            P1 = ~pattern;
            delay();
            pattern <<= 1;  /* Shift left */
        }

        /* Right shift: 1 LED moves right */
        pattern = 0x80;
        for (i = 0; i < 8; i++) {
            P1 = ~pattern;
            delay();
            pattern >>= 1;  /* Shift right */
        }

        /* Double shift: 2 LEDs move */
        pattern = 0x03;
        for (i = 0; i < 7; i++) {
            P1 = ~pattern;
            delay();
            pattern <<= 1;
        }
    }
}
