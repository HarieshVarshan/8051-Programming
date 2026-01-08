/*
 * 01_timer_delay.c - Timer-based Delays
 * Module 05: Timers
 *
 * Description: Precise delays using Timer 0
 * Hardware: LED on P1.0
 */

#include <8052.h>

__sbit __at (0x90) LED;

/*
 * Timer 0 delay in milliseconds
 * Mode 1 (16-bit), 12MHz crystal
 */
void timer0_delay_ms(unsigned int ms)
{
    unsigned int i;

    TMOD &= 0xF0;    /* Clear Timer 0 bits */
    TMOD |= 0x01;    /* Timer 0, Mode 1 (16-bit) */

    for (i = 0; i < ms; i++) {
        /* 1ms delay: 65536 - 1000 = 64536 = 0xFC18 */
        TH0 = 0xFC;
        TL0 = 0x18;
        TR0 = 1;        /* Start Timer 0 */
        while (!TF0);   /* Wait for overflow */
        TR0 = 0;        /* Stop Timer 0 */
        TF0 = 0;        /* Clear overflow flag */
    }
}

void main(void)
{
    while (1) {
        LED = 0;                  /* LED ON */
        timer0_delay_ms(500);    /* 500ms */

        LED = 1;                  /* LED OFF */
        timer0_delay_ms(500);    /* 500ms */
    }
}
