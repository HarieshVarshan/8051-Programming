/*
 * 03_keypad.c - 4x4 Matrix Keypad
 * Module 10: Motors & Projects
 *
 * Description: Scan 4x4 keypad and send key via serial
 * Hardware: Keypad on P2 (rows P2.0-3, cols P2.4-7)
 *           Serial connection to PC
 */

#include <8052.h>

#define KEYPAD_PORT P2

/* Row pins (output) */
__sbit __at (0xA0) ROW0;
__sbit __at (0xA1) ROW1;
__sbit __at (0xA2) ROW2;
__sbit __at (0xA3) ROW3;

/* Column pins (input) */
__sbit __at (0xA4) COL0;
__sbit __at (0xA5) COL1;
__sbit __at (0xA6) COL2;
__sbit __at (0xA7) COL3;

/* Keypad layout */
__code char KEYMAP[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

/* UART Functions */
void uart_init(void)
{
    TMOD = 0x20;
    TH1 = 0xFD;
    SCON = 0x50;
    TR1 = 1;
}

void uart_tx(unsigned char c)
{
    SBUF = c;
    while (!TI);
    TI = 0;
}

void uart_puts(char *str)
{
    while (*str) uart_tx(*str++);
}

/* Scan keypad and return key char (0 if none) */
char keypad_scan(void)
{
    unsigned char row;
    char key = 0;

    for (row = 0; row < 4; row++) {
        /* Set all rows high */
        KEYPAD_PORT = 0xFF;

        /* Set current row low */
        KEYPAD_PORT = ~(1 << row);

        /* Small delay for settling */
        delay_ms(1);

        /* Check columns */
        if (COL0 == 0) { key = KEYMAP[row][0]; break; }
        if (COL1 == 0) { key = KEYMAP[row][1]; break; }
        if (COL2 == 0) { key = KEYMAP[row][2]; break; }
        if (COL3 == 0) { key = KEYMAP[row][3]; break; }
    }

    /* Reset port */
    KEYPAD_PORT = 0xFF;

    return key;
}

/* Wait for key press with debounce */
char keypad_getkey(void)
{
    char key1, key2;

    /* Wait for key press */
    do {
        key1 = keypad_scan();
    } while (key1 == 0);

    /* Debounce */
    delay_ms(20);

    /* Verify */
    key2 = keypad_scan();
    if (key1 != key2) return 0;

    /* Wait for key release */
    while (keypad_scan() != 0);
    delay_ms(20);

    return key1;
}

void main(void)
{
    char key;

    uart_init();
    KEYPAD_PORT = 0xFF;  /* All high (pull-ups) */

    uart_puts("4x4 Keypad Test\r\n");
    uart_puts("===============\r\n\r\n");
    uart_puts("Press any key...\r\n");

    while (1) {
        key = keypad_getkey();

        if (key != 0) {
            uart_puts("Key: ");
            uart_tx(key);
            uart_puts("\r\n");
        }
    }
}
