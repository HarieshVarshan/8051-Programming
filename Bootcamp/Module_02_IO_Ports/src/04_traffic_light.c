/*
 * 04_traffic_light.c - Simple Traffic Light
 * Module 02: I/O Port Programming
 *
 * Description: 3-LED traffic light sequence
 * Hardware:
 *   P1.0 - Red LED
 *   P1.1 - Yellow LED
 *   P1.2 - Green LED
 */

#include <8052.h>

__sbit __at (0x90) RED;
__sbit __at (0x91) YELLOW;
__sbit __at (0x92) GREEN;

/* LED states (active low) */
#define ON  0
#define OFF 1

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 120; j++);
    }
}

void all_off(void)
{
    RED = OFF;
    YELLOW = OFF;
    GREEN = OFF;
}

void main(void)
{
    all_off();

    while (1) {
        /* Red light - Stop (3 seconds) */
        all_off();
        RED = ON;
        delay_ms(3000);

        /* Red + Yellow - Get ready (1 second) */
        YELLOW = ON;
        delay_ms(1000);

        /* Green light - Go (3 seconds) */
        all_off();
        GREEN = ON;
        delay_ms(3000);

        /* Yellow light - Slow down (1 second) */
        all_off();
        YELLOW = ON;
        delay_ms(1000);
    }
}
