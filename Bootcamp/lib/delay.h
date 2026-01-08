/*
 * delay.h - Delay Functions Library
 * 8051 Bootcamp Shared Library
 *
 * Usage: #include "../lib/delay.h"
 *
 * Functions provide software delays for 12MHz crystal.
 * For other frequencies, adjust loop counts accordingly.
 */

#ifndef DELAY_H
#define DELAY_H

/*
 * Microsecond delay (approximate)
 * Each iteration is roughly 1us at 12MHz
 *
 * @param us: Number of microseconds to delay
 */
void delay_us(unsigned int us)
{
    while (us--);
}

/*
 * Millisecond delay
 * Calibrated for 12MHz crystal
 *
 * @param ms: Number of milliseconds to delay
 */
void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

/*
 * Second delay
 * Uses delay_ms internally
 *
 * @param sec: Number of seconds to delay
 */
void delay_sec(unsigned int sec)
{
    unsigned int i;
    for (i = 0; i < sec; i++)
        delay_ms(1000);
}

#endif /* DELAY_H */
