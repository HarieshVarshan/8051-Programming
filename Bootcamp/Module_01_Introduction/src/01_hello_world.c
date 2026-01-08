/*
 * 01_hello_world.c - Your First 8051 Program
 * Module 01: Introduction to 8051
 *
 * Description: Blinks all LEDs on Port 1
 * Hardware: 8 LEDs connected to P1.0-P1.7 (active low)
 *
 * Circuit:
 *   P1.x ──[220Ω]──►LED──GND
 *
 * Build: make
 * Output: build/01_hello_world.ihx
 */

#include <8052.h>

/*
 * Simple delay function
 * Approximate delay depends on clock frequency
 * At 12MHz: ~500ms delay
 */
void delay(void)
{
    unsigned int i, j;
    for (i = 0; i < 100; i++) {
        for (j = 0; j < 1000; j++) {
            /* Empty loop for delay */
        }
    }
}

/*
 * Main function - Entry point
 */
void main(void)
{
    /* Infinite loop - microcontrollers run forever */
    while (1) {
        P1 = 0x00;    /* All bits LOW = LEDs ON (active low) */
        delay();

        P1 = 0xFF;    /* All bits HIGH = LEDs OFF */
        delay();
    }
}
