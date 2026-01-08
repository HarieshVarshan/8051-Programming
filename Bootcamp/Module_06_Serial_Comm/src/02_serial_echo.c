/*
 * 02_serial_echo.c - Serial Echo
 * Module 06: Serial Communication
 *
 * Description: Echo received characters back
 * Baud: 9600, 8N1
 */

#include <8052.h>

void uart_init(void)
{
    TMOD = 0x20;    /* Timer 1, Mode 2 */
    TH1 = 0xFD;     /* 9600 baud */
    SCON = 0x50;    /* Mode 1, REN enabled */
    TR1 = 1;
}

void uart_tx(unsigned char c)
{
    SBUF = c;
    while (!TI);
    TI = 0;
}

unsigned char uart_rx(void)
{
    while (!RI);    /* Wait for receive */
    RI = 0;         /* Clear flag */
    return SBUF;    /* Return received byte */
}

void uart_puts(char *str)
{
    while (*str) {
        uart_tx(*str++);
    }
}

void main(void)
{
    unsigned char c;

    uart_init();
    uart_puts("Echo Test - Type something:\r\n");

    while (1) {
        c = uart_rx();      /* Wait for character */
        uart_tx(c);         /* Echo it back */

        if (c == '\r') {
            uart_tx('\n');  /* Add newline after carriage return */
        }
    }
}
