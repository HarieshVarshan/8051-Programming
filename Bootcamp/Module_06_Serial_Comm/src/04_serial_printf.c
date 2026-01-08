/*
 * 04_serial_printf.c - Formatted Output
 * Module 06: Serial Communication
 *
 * Description: Send formatted numbers via serial
 * Hardware: Serial connection to PC
 */

#include <8052.h>

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

/* Send unsigned char as decimal */
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

/* Send unsigned char as hex */
void uart_puthex(unsigned char num)
{
    unsigned char nibble;

    uart_puts("0x");

    nibble = (num >> 4) & 0x0F;
    uart_tx(nibble < 10 ? '0' + nibble : 'A' + nibble - 10);

    nibble = num & 0x0F;
    uart_tx(nibble < 10 ? '0' + nibble : 'A' + nibble - 10);
}

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

void main(void)
{
    unsigned char counter = 0;

    uart_init();
    uart_puts("Number Display Demo\r\n");
    uart_puts("===================\r\n\r\n");

    while (1) {
        uart_puts("Counter: ");
        uart_putnum(counter);
        uart_puts(" (");
        uart_puthex(counter);
        uart_puts(")\r\n");

        counter++;
        delay_ms(500);
    }
}
