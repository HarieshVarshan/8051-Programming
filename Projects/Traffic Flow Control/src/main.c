/*
 * Traffic Flow Control System
 *
 * Description: 4-way traffic light controller with countdown display
 * Target: 8051 microcontroller @ 12MHz
 * Compiler: SDCC (Small Device C Compiler)
 *
 * Hardware Configuration:
 *   P0 - Traffic lights for Road 1 & 2 (Red, Yellow, Green LEDs)
 *   P1 - Traffic lights for Road 3 & 4 (Red, Yellow, Green LEDs)
 *   P2 - Seven-segment display (tens digit) for countdown timer
 *   P3 - Seven-segment display (units digit) for countdown timer
 *
 * Traffic Light Bit Mapping (active high):
 *   Bit 0: Road 1 Green    Bit 4: Road 2 Green
 *   Bit 1: Road 1 Yellow   Bit 5: Road 2 Yellow
 *   Bit 2: Road 1 Red      Bit 6: Road 2 Red
 *
 * Build: make
 * Output: build/traffic.ihx
 */

#include <8052.h>

/* Timing Configuration */
#define DELAY_OUTER_LOOP    50
#define DELAY_INNER_LOOP    1275

/* Traffic Light States (P0/P1 patterns) */
#define ROAD1_GREEN_ROAD2_RED    0x44
#define ROAD1_RED_ROAD2_GREEN    0x11
#define ROAD1_YELLOW_ROAD2_RED   0x22
#define ROAD1_RED_ROAD2_YELLOW   0x88

/* Countdown Timer Ranges */
#define COUNTDOWN_LONG     4    /* 40-49 seconds for green phase */
#define COUNTDOWN_SHORT    0    /* 0-9 seconds for yellow phase */

#define NUM_STATES         8

/* Seven-segment display patterns (common cathode, 0-9) - stored in ROM */
__code unsigned char SEGMENT_PATTERNS[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x67
};

/* Traffic light sequence for all 4 roads (pairs of P0, P1 values) */
__code unsigned char TRAFFIC_SEQUENCE[][2] = {
    {ROAD1_GREEN_ROAD2_RED,   ROAD1_RED_ROAD2_GREEN},
    {ROAD1_GREEN_ROAD2_RED,   ROAD1_YELLOW_ROAD2_RED},
    {ROAD1_RED_ROAD2_GREEN,   ROAD1_GREEN_ROAD2_RED},
    {ROAD1_YELLOW_ROAD2_RED,  ROAD1_GREEN_ROAD2_RED},
    {ROAD1_RED_ROAD2_YELLOW,  ROAD1_RED_ROAD2_GREEN},
    {ROAD1_RED_ROAD2_YELLOW,  ROAD1_YELLOW_ROAD2_RED},
    {ROAD1_YELLOW_ROAD2_RED,  ROAD1_RED_ROAD2_YELLOW},
    {ROAD1_RED_ROAD2_GREEN,   ROAD1_RED_ROAD2_YELLOW}
};

/* Generate approximately 1 second delay at 12MHz */
void delay_1sec(void)
{
    unsigned int i, j;
    for (i = 0; i < DELAY_OUTER_LOOP; i++) {
        for (j = 0; j < DELAY_INNER_LOOP; j++);
    }
}

/* Display countdown on seven-segment displays */
void display_countdown(unsigned char start_tens, unsigned char is_green)
{
    signed char tens, units;
    unsigned char end_tens = is_green ? 1 : 0;

    for (tens = start_tens; tens >= (signed char)end_tens; tens--) {
        for (units = 9; units >= 0; units--) {
            P2 = SEGMENT_PATTERNS[tens];
            P3 = SEGMENT_PATTERNS[units];
            delay_1sec();
        }
    }
}

void main(void)
{
    unsigned char state;
    unsigned char is_green_phase = 1;

    /* Initialize all ports */
    P0 = P1 = P2 = P3 = 0x00;

    while (1) {
        for (state = 0; state < NUM_STATES; state++) {
            P0 = TRAFFIC_SEQUENCE[state][0];
            P1 = TRAFFIC_SEQUENCE[state][1];

            if (is_green_phase) {
                display_countdown(COUNTDOWN_LONG, 1);
            } else {
                display_countdown(COUNTDOWN_SHORT, 0);
            }

            is_green_phase = !is_green_phase;
        }
    }
}
