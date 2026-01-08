/*
 * 03_button_debounce.c - Debounced Button
 * Module 04: Loops & Functions
 *
 * Description: Proper button handling with debounce
 * Hardware: Button on P3.2, LED on P1.0
 */

#include <8052.h>

__sbit __at (0x90) LED;
__sbit __at (0xB2) BUTTON;  /* P3.2 (INT0 pin) */

#define DEBOUNCE_MS 20

void delay_ms(unsigned int ms)
{
    unsigned int i;
    while (ms--) {
        for (i = 0; i < 120; i++);
    }
}

unsigned char debounce_button(void)
{
    if (BUTTON == 0) {           /* Button pressed? */
        delay_ms(DEBOUNCE_MS);   /* Wait for debounce */
        if (BUTTON == 0) {       /* Still pressed? */
            while (BUTTON == 0); /* Wait for release */
            delay_ms(DEBOUNCE_MS);
            return 1;            /* Valid press */
        }
    }
    return 0;                    /* No press */
}

void main(void)
{
    unsigned char led_state = 1;  /* OFF initially */

    BUTTON = 1;  /* Configure as input */
    LED = led_state;

    while (1) {
        if (debounce_button()) {
            /* Toggle LED on each button press */
            led_state = !led_state;
            LED = led_state;
        }
    }
}
