/*
 * 01_led_blink.c - Single LED Blinking
 * Module 02: I/O Port Programming
 *
 * Description: Blinks a single LED on P1.0
 * Hardware: LED connected to P1.0 (active low)
 */

#include <8052.h>

/* Define LED pin using sbit */
__sbit __at (0x90) LED;  /* P1.0 */

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 120; j++);
    }
}

void main(void)
{
    while (1) {
        LED = 0;          /* LED ON (active low) */
        delay_ms(500);    /* 500ms delay */

        LED = 1;          /* LED OFF */
        delay_ms(500);
    }
}
