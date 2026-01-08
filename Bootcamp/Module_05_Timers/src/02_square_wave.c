/*
 * 02_square_wave.c - Square Wave Generator
 * Module 05: Timers
 *
 * Description: Generate 1kHz square wave on P1.0
 * Hardware: Connect oscilloscope or speaker to P1.0
 */

#include <8052.h>

__sbit __at (0x90) WAVE_OUT;

/*
 * For 1kHz wave: period = 1ms, half-period = 500µs
 * Timer value: 65536 - 500 = 65036 = 0xFE0C
 */
void main(void)
{
    TMOD = 0x01;    /* Timer 0, Mode 1 */

    while (1) {
        TH0 = 0xFE;
        TL0 = 0x0C;
        TR0 = 1;

        while (!TF0);   /* Wait for overflow (500µs) */

        TR0 = 0;
        TF0 = 0;
        WAVE_OUT = ~WAVE_OUT;   /* Toggle output */
    }
}
