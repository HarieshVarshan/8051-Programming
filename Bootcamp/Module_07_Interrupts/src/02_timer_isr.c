/*
 * 02_timer_isr.c - Timer Interrupt LED Blink
 * Module 07: Interrupts
 *
 * Description: Blink LED every 1 second using Timer 0 interrupt
 * Hardware: LED on P1.0
 * Timer: 50ms interrupt, count 20 for 1 second
 */

#include <8052.h>

/* Volatile - shared between ISR and main */
volatile unsigned char tick_count = 0;
volatile unsigned char second_flag = 0;

/* Timer 0 ISR - fires every 50ms */
void timer0_isr(void) __interrupt(1)
{
    /* Reload timer for 50ms @ 12MHz */
    /* 50ms = 50000us, need 65536 - 50000 = 15536 = 0x3CB0 */
    TH0 = 0x3C;
    TL0 = 0xB0;

    tick_count++;
    if (tick_count >= 20) {  /* 20 x 50ms = 1 second */
        tick_count = 0;
        second_flag = 1;
    }
}

void timer0_init(void)
{
    TMOD = 0x01;    /* Timer 0, Mode 1 (16-bit) */
    TH0 = 0x3C;     /* Initial value for 50ms */
    TL0 = 0xB0;
    ET0 = 1;        /* Enable Timer 0 interrupt */
    EA = 1;         /* Global interrupt enable */
    TR0 = 1;        /* Start Timer 0 */
}

void main(void)
{
    unsigned char led_state = 0;

    timer0_init();
    P1 = 0xFF;  /* All LEDs OFF */

    while (1) {
        if (second_flag) {
            second_flag = 0;

            /* Toggle LED */
            led_state = !led_state;
            P1_0 = led_state;
        }

        /* Main loop is free for other tasks */
        /* CPU is not blocked by delay loops */
    }
}
