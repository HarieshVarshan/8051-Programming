/*
 * Digital Clock
 * 24-hour clock with 4-digit 7-segment display
 *
 * Hardware:
 *   - 4-digit 7-segment (CC) on P1 (segments), P2.0-3 (digits)
 *   - Buttons: P3.2 (Mode), P3.3 (Up), P3.4 (Down)
 *   - Crystal: 11.0592MHz for accurate timing
 */

#include <8052.h>

/* Pin Definitions */
#define SEGMENTS P1
#define DIGITS   P2

__sbit __at (0xB2) BTN_MODE;   /* P3.2 */
__sbit __at (0xB3) BTN_UP;     /* P3.3 */
__sbit __at (0xB4) BTN_DOWN;   /* P3.4 */

/* 7-Segment patterns (Common Cathode) */
__code unsigned char SEG_PATTERN[] = {
    0x3F,  /* 0 */
    0x06,  /* 1 */
    0x5B,  /* 2 */
    0x4F,  /* 3 */
    0x66,  /* 4 */
    0x6D,  /* 5 */
    0x7D,  /* 6 */
    0x07,  /* 7 */
    0x7F,  /* 8 */
    0x6F   /* 9 */
};

/* Digit select patterns (active low) */
__code unsigned char DIGIT_SEL[] = {
    0xFE,  /* Digit 0 */
    0xFD,  /* Digit 1 */
    0xFB,  /* Digit 2 */
    0xF7   /* Digit 3 */
};

/* Operating modes */
#define MODE_NORMAL     0
#define MODE_SET_HOUR   1
#define MODE_SET_MIN    2

/* Time variables (volatile - modified in ISR) */
volatile unsigned char hours = 12;
volatile unsigned char minutes = 0;
volatile unsigned char seconds = 0;
volatile unsigned char tick_count = 0;
volatile unsigned char second_flag = 0;

/* Display variables */
unsigned char display[4] = {1, 2, 0, 0};
unsigned char current_mode = MODE_NORMAL;
unsigned char blink_state = 0;
unsigned char blink_counter = 0;

/* Button state */
unsigned char mode_prev = 1;
unsigned char up_prev = 1;
unsigned char down_prev = 1;

/*
 * Timer 0 ISR - 50ms interrupt
 * 20 interrupts = 1 second
 */
void timer0_isr(void) __interrupt(1)
{
    /* Reload timer for 50ms @ 11.0592MHz */
    TH0 = 0x4C;
    TL0 = 0x00;

    tick_count++;
    if (tick_count >= 20) {
        tick_count = 0;
        second_flag = 1;

        /* Increment time */
        seconds++;
        if (seconds >= 60) {
            seconds = 0;
            minutes++;
            if (minutes >= 60) {
                minutes = 0;
                hours++;
                if (hours >= 24) {
                    hours = 0;
                }
            }
        }
    }

    /* Blink counter for set mode */
    blink_counter++;
    if (blink_counter >= 5) {
        blink_counter = 0;
        blink_state = !blink_state;
    }
}

void timer_init(void)
{
    TMOD = (TMOD & 0xF0) | 0x01;  /* Timer 0, Mode 1 */
    TH0 = 0x4C;
    TL0 = 0x00;
    ET0 = 1;    /* Enable Timer 0 interrupt */
    EA = 1;     /* Global interrupt enable */
    TR0 = 1;    /* Start Timer 0 */
}

void delay_us(unsigned int us)
{
    while (us--);
}

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 100; j++);
}

/* Update display buffer from time */
void update_display(void)
{
    display[0] = hours / 10;
    display[1] = hours % 10;
    display[2] = minutes / 10;
    display[3] = minutes % 10;
}

/* Refresh display (call frequently) */
void refresh_display(void)
{
    unsigned char i;
    unsigned char pattern;
    unsigned char show_digit;

    for (i = 0; i < 4; i++) {
        DIGITS = 0xFF;  /* All digits off */

        /* Determine if digit should be shown (for blinking) */
        show_digit = 1;
        if (current_mode == MODE_SET_HOUR && (i == 0 || i == 1)) {
            show_digit = blink_state;
        }
        if (current_mode == MODE_SET_MIN && (i == 2 || i == 3)) {
            show_digit = blink_state;
        }

        if (show_digit) {
            pattern = SEG_PATTERN[display[i]];

            /* Add colon (decimal point on digit 1) */
            if (i == 1 && current_mode == MODE_NORMAL) {
                if (blink_state) {
                    pattern |= 0x80;  /* DP on */
                }
            }

            SEGMENTS = pattern;
        } else {
            SEGMENTS = 0x00;  /* Blank */
        }

        DIGITS = DIGIT_SEL[i];
        delay_us(500);
    }

    DIGITS = 0xFF;  /* All off */
}

/* Check button press (with debounce) */
unsigned char button_pressed(unsigned char *prev, unsigned char current)
{
    unsigned char pressed = 0;

    if (current == 0 && *prev == 1) {
        delay_ms(20);  /* Debounce */
        if (current == 0) {
            pressed = 1;
        }
    }
    *prev = current;

    return pressed;
}

/* Handle mode button */
void handle_mode(void)
{
    if (button_pressed(&mode_prev, BTN_MODE)) {
        current_mode++;
        if (current_mode > MODE_SET_MIN) {
            current_mode = MODE_NORMAL;
        }
    }
}

/* Handle up/down buttons */
void handle_adjust(void)
{
    if (current_mode == MODE_SET_HOUR) {
        if (button_pressed(&up_prev, BTN_UP)) {
            hours++;
            if (hours >= 24) hours = 0;
        }
        if (button_pressed(&down_prev, BTN_DOWN)) {
            if (hours == 0) hours = 23;
            else hours--;
        }
    }
    else if (current_mode == MODE_SET_MIN) {
        if (button_pressed(&up_prev, BTN_UP)) {
            minutes++;
            if (minutes >= 60) minutes = 0;
            seconds = 0;  /* Reset seconds when setting */
        }
        if (button_pressed(&down_prev, BTN_DOWN)) {
            if (minutes == 0) minutes = 59;
            else minutes--;
            seconds = 0;
        }
    }
}

void main(void)
{
    /* Initialize */
    SEGMENTS = 0x00;
    DIGITS = 0xFF;

    timer_init();

    /* Initial time: 12:00 */
    hours = 12;
    minutes = 0;
    seconds = 0;

    while (1) {
        /* Handle buttons */
        handle_mode();
        handle_adjust();

        /* Update display buffer */
        update_display();

        /* Refresh display */
        refresh_display();
    }
}
