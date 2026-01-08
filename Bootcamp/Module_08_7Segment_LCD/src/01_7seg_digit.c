/*
 * 01_7seg_digit.c - Single 7-Segment Digit
 * Module 08: 7-Segment & LCD
 *
 * Description: Display digits 0-9 on single 7-segment
 * Hardware: Common cathode 7-segment on P1
 */

#include <8052.h>

/* 7-segment patterns for 0-9 (Common Cathode) */
/* Bit order: dp g f e d c b a */
__code unsigned char SEGMENT[] = {
    0x3F,   /* 0: a b c d e f    */
    0x06,   /* 1:   b c          */
    0x5B,   /* 2: a b   d e   g  */
    0x4F,   /* 3: a b c d     g  */
    0x66,   /* 4:   b c   f g    */
    0x6D,   /* 5: a   c d   f g  */
    0x7D,   /* 6: a   c d e f g  */
    0x07,   /* 7: a b c          */
    0x7F,   /* 8: a b c d e f g  */
    0x6F    /* 9: a b c d   f g  */
};

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

void main(void)
{
    unsigned char digit = 0;

    while (1) {
        /* Display current digit */
        P1 = SEGMENT[digit];

        /* Wait 1 second */
        delay_ms(1000);

        /* Next digit (0-9) */
        digit++;
        if (digit > 9) digit = 0;
    }
}
