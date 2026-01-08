/*
 * 04_binary_counter.c - 8-bit Binary Counter
 * Module 03: Arithmetic & Logic
 *
 * Description: Counts 0-255 on LEDs
 * Hardware: 8 LEDs on P1
 */

#include <8052.h>

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

void main(void)
{
    unsigned char count = 0;

    while (1) {
        P1 = ~count;      /* Display count on LEDs */
        count++;          /* Increment (wraps at 255→0) */
        delay_ms(200);    /* 200ms between counts */
    }
}
