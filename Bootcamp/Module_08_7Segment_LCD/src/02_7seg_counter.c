/*
 * 02_7seg_counter.c - 4-Digit Multiplexed Counter
 * Module 08: 7-Segment & LCD
 *
 * Description: 0000-9999 counter on multiplexed display
 * Hardware: 4 common cathode 7-segments
 *           Segments on P1, Digit select on P2.0-P2.3
 */

#include <8052.h>

/* 7-segment patterns (Common Cathode) */
__code unsigned char SEGMENT[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

/* Digit select patterns (active low) */
__code unsigned char DIGIT_SEL[] = {
    0xFE,   /* Digit 1: P2.0 = 0 */
    0xFD,   /* Digit 2: P2.1 = 0 */
    0xFB,   /* Digit 3: P2.2 = 0 */
    0xF7    /* Digit 4: P2.3 = 0 */
};

/* Display buffer */
unsigned char display[4] = {0, 0, 0, 0};

void delay_us(unsigned int us)
{
    while (us--);
}

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

/* Refresh display once (call frequently) */
void refresh_display(void)
{
    unsigned char i;

    for (i = 0; i < 4; i++) {
        P2 = 0xFF;                  /* All digits off */
        P1 = SEGMENT[display[i]];   /* Set segment pattern */
        P2 = DIGIT_SEL[i];          /* Enable digit */
        delay_us(500);              /* Display time ~2ms */
    }
    P2 = 0xFF;  /* All digits off */
}

/* Convert number to display digits */
void set_number(unsigned int num)
{
    display[0] = num / 1000;
    display[1] = (num / 100) % 10;
    display[2] = (num / 10) % 10;
    display[3] = num % 10;
}

void main(void)
{
    unsigned int counter = 0;
    unsigned int refresh_count = 0;

    P1 = 0x00;
    P2 = 0xFF;

    while (1) {
        /* Update display buffer */
        set_number(counter);

        /* Refresh display many times before incrementing */
        for (refresh_count = 0; refresh_count < 50; refresh_count++) {
            refresh_display();
        }

        /* Increment counter */
        counter++;
        if (counter > 9999) counter = 0;
    }
}
