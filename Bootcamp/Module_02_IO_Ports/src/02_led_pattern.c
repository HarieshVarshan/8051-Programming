/*
 * 02_led_pattern.c - Knight Rider LED Effect
 * Module 02: I/O Port Programming
 *
 * Description: Running LED pattern (left-right-left)
 * Hardware: 8 LEDs connected to P1.0-P1.7
 */

#include <8052.h>

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 120; j++);
    }
}

void main(void)
{
    unsigned char pattern = 0x01;
    unsigned char direction = 0;  /* 0=left, 1=right */

    while (1) {
        P1 = ~pattern;    /* Invert for active-low LEDs */
        delay_ms(100);

        if (direction == 0) {
            /* Shift left */
            pattern <<= 1;
            if (pattern == 0x80) {
                direction = 1;  /* Change direction */
            }
        } else {
            /* Shift right */
            pattern >>= 1;
            if (pattern == 0x01) {
                direction = 0;  /* Change direction */
            }
        }
    }
}
