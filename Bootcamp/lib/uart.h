/*
 * uart.h - UART Serial Communication Library
 * 8051 Bootcamp Shared Library
 *
 * Usage: #include "../lib/uart.h"
 *
 * Default configuration: 9600 baud, 8N1
 * Requires 11.0592MHz crystal for accurate baud rates
 */

#ifndef UART_H
#define UART_H

#include <8052.h>

/* Baud rate settings for 11.0592MHz crystal */
#define BAUD_9600   0xFD
#define BAUD_4800   0xFA
#define BAUD_2400   0xF4
#define BAUD_1200   0xE8

/*
 * Initialize UART
 * Timer 1 in Mode 2 (auto-reload)
 * Default: 9600 baud, 8N1
 */
void uart_init(void)
{
    TMOD = (TMOD & 0x0F) | 0x20;  /* Timer 1, Mode 2 */
    TH1 = BAUD_9600;              /* 9600 baud */
    SCON = 0x50;                  /* Mode 1, REN enabled */
    TR1 = 1;                      /* Start Timer 1 */
}

/*
 * Initialize UART with specific baud rate
 *
 * @param baud_val: TH1 value (use BAUD_xxxx defines)
 */
void uart_init_baud(unsigned char baud_val)
{
    TMOD = (TMOD & 0x0F) | 0x20;
    TH1 = baud_val;
    SCON = 0x50;
    TR1 = 1;
}

/*
 * Transmit single character
 *
 * @param c: Character to transmit
 */
void uart_tx(unsigned char c)
{
    SBUF = c;
    while (!TI);
    TI = 0;
}

/*
 * Receive single character (blocking)
 *
 * @return: Received character
 */
unsigned char uart_rx(void)
{
    while (!RI);
    RI = 0;
    return SBUF;
}

/*
 * Check if data available
 *
 * @return: 1 if data available, 0 otherwise
 */
unsigned char uart_available(void)
{
    return RI;
}

/*
 * Receive character (non-blocking)
 *
 * @return: Received character, or 0 if none available
 */
unsigned char uart_rx_nb(void)
{
    if (!RI) return 0;
    RI = 0;
    return SBUF;
}

/*
 * Transmit string
 *
 * @param str: Null-terminated string
 */
void uart_puts(char *str)
{
    while (*str) {
        uart_tx(*str++);
    }
}

/*
 * Transmit unsigned char as decimal
 *
 * @param num: Number to transmit (0-255)
 */
void uart_putnum(unsigned char num)
{
    unsigned char hundreds, tens, ones;

    hundreds = num / 100;
    tens = (num / 10) % 10;
    ones = num % 10;

    if (hundreds > 0) uart_tx('0' + hundreds);
    if (hundreds > 0 || tens > 0) uart_tx('0' + tens);
    uart_tx('0' + ones);
}

/*
 * Transmit unsigned char as hex (0xNN format)
 *
 * @param num: Number to transmit
 */
void uart_puthex(unsigned char num)
{
    unsigned char nibble;

    uart_puts("0x");

    nibble = (num >> 4) & 0x0F;
    uart_tx(nibble < 10 ? '0' + nibble : 'A' + nibble - 10);

    nibble = num & 0x0F;
    uart_tx(nibble < 10 ? '0' + nibble : 'A' + nibble - 10);
}

/*
 * Transmit newline (CR+LF)
 */
void uart_newline(void)
{
    uart_tx('\r');
    uart_tx('\n');
}

#endif /* UART_H */
