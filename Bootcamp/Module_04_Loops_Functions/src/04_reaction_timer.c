/*
 * 04_reaction_timer.c - Reaction Time Game
 * Module 04: Loops & Functions
 *
 * Description: Measure reaction time
 * Hardware: Button on P3.2, LEDs on P1
 */

#include <8052.h>

__sbit __at (0xB2) BUTTON;

void delay_ms(unsigned int ms)
{
    unsigned int i;
    while (ms--) {
        for (i = 0; i < 120; i++);
    }
}

unsigned int random_delay(void)
{
    /* Simple pseudo-random delay 1-5 seconds */
    static unsigned int seed = 12345;
    seed = seed * 1103515245 + 12345;
    return 1000 + (seed % 4000);  /* 1000-5000 ms */
}

void main(void)
{
    unsigned int reaction_time;
    unsigned int i;

    BUTTON = 1;  /* Input */

    while (1) {
        /* All LEDs OFF - waiting */
        P1 = 0xFF;
        delay_ms(random_delay());

        /* LED ON - Start timing! */
        P1 = 0x00;
        reaction_time = 0;

        /* Count until button pressed */
        while (BUTTON == 1) {
            delay_ms(10);
            reaction_time += 10;
            if (reaction_time > 2000) break;  /* Timeout */
        }

        /* Display result on LEDs (lower = faster) */
        /* Scale: 0-255 mapped to reaction time */
        P1 = ~(unsigned char)(reaction_time / 8);

        /* Wait for button release and delay */
        while (BUTTON == 0);
        delay_ms(2000);
    }
}
