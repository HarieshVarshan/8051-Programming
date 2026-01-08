/*
 * 04_event_counter.c - External Event Counter
 * Module 05: Timers
 *
 * Description: Count button presses using Timer as counter
 * Hardware: Button on T0 (P3.4), count displayed on P1
 */

#include <8052.h>

void main(void)
{
    /* Timer 0 as Counter, Mode 1 (16-bit) */
    TMOD = 0x05;    /* C/T=1 (counter), M1M0=01 (mode 1) */

    TH0 = 0x00;     /* Start from 0 */
    TL0 = 0x00;
    TR0 = 1;        /* Start counter */

    while (1) {
        /* Display low byte of count on LEDs */
        P1 = ~TL0;

        /* Check for overflow (255 presses) */
        if (TF0) {
            TF0 = 0;
            /* Counter overflowed - could trigger action */
        }
    }
}
