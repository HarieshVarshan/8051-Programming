/*
 * 04_stopwatch.c - Simple Stopwatch
 * Module 07: Interrupts
 *
 * Description: Stopwatch with start/stop button
 * Hardware: Button on P3.2 (INT0), LEDs on P1
 * Display: Binary seconds on P1 (0-255)
 */

#include <8052.h>

/* Stopwatch state */
volatile unsigned char running = 0;
volatile unsigned char seconds = 0;
volatile unsigned char tick_count = 0;

/* Button press flag */
volatile unsigned char button_flag = 0;

/* External INT0 - Start/Stop button */
void ext0_isr(void) __interrupt(0)
{
    button_flag = 1;
}

/* Timer 0 - 10ms tick */
void timer0_isr(void) __interrupt(1)
{
    /* Reload for 10ms @ 12MHz */
    TH0 = 0xD8;
    TL0 = 0xF0;

    if (running) {
        tick_count++;
        if (tick_count >= 100) {  /* 100 x 10ms = 1 second */
            tick_count = 0;
            seconds++;
        }
    }
}

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

void system_init(void)
{
    /* Timer 0: Mode 1, 16-bit */
    TMOD = 0x01;
    TH0 = 0xD8;     /* 10ms delay value */
    TL0 = 0xF0;

    /* INT0: Edge triggered */
    IT0 = 1;

    /* Enable interrupts */
    ET0 = 1;        /* Timer 0 interrupt */
    EX0 = 1;        /* External INT0 */
    EA = 1;         /* Global enable */

    /* Start Timer 0 */
    TR0 = 1;
}

void main(void)
{
    system_init();

    P1 = 0xFF;      /* Display OFF */
    seconds = 0;
    running = 0;

    while (1) {
        /* Handle button press */
        if (button_flag) {
            delay_ms(50);  /* Debounce */
            button_flag = 0;

            /* Toggle running state */
            running = !running;

            /* Reset on start */
            if (running) {
                seconds = 0;
                tick_count = 0;
            }
        }

        /* Update display (inverted for common cathode LEDs) */
        P1 = ~seconds;

        /* LED P1.7 shows running status */
        /* Blinks when stopped, steady when running */
        if (!running) {
            static unsigned int blink = 0;
            blink++;
            if (blink > 5000) {
                blink = 0;
                P1_7 = !P1_7;
            }
        }
    }
}
