/*
 * 01_button_isr.c - External Interrupt Button
 * Module 07: Interrupts
 *
 * Description: Toggle LED on button press using INT0
 * Hardware: Button on P3.2 (INT0), LED on P1.0
 */

#include <8052.h>

/* Volatile - shared between ISR and main */
volatile unsigned char button_pressed = 0;

/* External INT0 ISR */
void ext0_isr(void) __interrupt(0)
{
    button_pressed = 1;  /* Set flag for main loop */
}

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

void main(void)
{
    /* Configure INT0 */
    IT0 = 1;    /* Edge triggered (falling edge) */
    EX0 = 1;    /* Enable External Interrupt 0 */
    EA = 1;     /* Global interrupt enable */

    P1 = 0xFF;  /* All LEDs OFF initially */

    while (1) {
        if (button_pressed) {
            /* Debounce delay */
            delay_ms(50);

            /* Toggle LED on P1.0 */
            P1_0 = !P1_0;

            /* Clear flag */
            button_pressed = 0;
        }

        /* Main loop can do other work here */
    }
}
