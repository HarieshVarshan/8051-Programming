/*
 * 01_delay_functions.c - Calibrated Delays
 * Module 04: Loops & Functions
 *
 * Description: Reusable delay functions
 * Hardware: LED on P1.0
 */

#include <8052.h>

__sbit __at (0x90) LED;

/* Microsecond delay (approximate) */
void delay_us(unsigned int us)
{
    while (us--) {
        /* ~1µs at 12MHz */
    }
}

/* Millisecond delay */
void delay_ms(unsigned int ms)
{
    unsigned int i;
    while (ms--) {
        for (i = 0; i < 120; i++);
    }
}

/* Second delay */
void delay_sec(unsigned char sec)
{
    while (sec--) {
        delay_ms(1000);
    }
}

void main(void)
{
    while (1) {
        LED = 0;
        delay_sec(1);    /* ON for 1 second */

        LED = 1;
        delay_ms(500);   /* OFF for 500ms */

        LED = 0;
        delay_ms(200);   /* ON for 200ms */

        LED = 1;
        delay_ms(200);   /* OFF for 200ms */
    }
}
