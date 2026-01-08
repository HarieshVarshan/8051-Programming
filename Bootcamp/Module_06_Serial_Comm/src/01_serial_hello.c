/*
 * 01_serial_hello.c - Serial Hello World
 * Module 06: Serial Communication
 *
 * Description: Send "Hello World" via UART
 * Hardware: Connect to PC via MAX232 or USB-Serial
 * Baud: 9600, 8N1
 */

#include <8052.h>

void uart_init(void)
{
    TMOD = 0x20;    /* Timer 1, Mode 2 (auto-reload) */
    TH1 = 0xFD;     /* 9600 baud @ 11.0592MHz */
    SCON = 0x50;    /* Mode 1, REN enabled */
    TR1 = 1;        /* Start Timer 1 */
}

void uart_tx(unsigned char c)
{
    SBUF = c;           /* Load character */
    while (!TI);        /* Wait for transmit complete */
    TI = 0;             /* Clear flag */
}

void uart_puts(char *str)
{
    while (*str) {
        uart_tx(*str++);
    }
}

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

void main(void)
{
    uart_init();

    while (1) {
        uart_puts("Hello, 8051 World!\r\n");
        delay_ms(1000);
    }
}
