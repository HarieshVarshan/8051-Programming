/*
 * 03_serial_menu.c - Interactive Menu
 * Module 06: Serial Communication
 *
 * Description: Menu-driven LED control via serial
 * Hardware: LEDs on P1, Serial connection
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

unsigned char uart_rx(void)
{
    while (!RI);
    RI = 0;
    return SBUF;
}

void uart_puts(char *str)
{
    while (*str) uart_tx(*str++);
}

void show_menu(void)
{
    uart_puts("\r\n=== LED Control Menu ===\r\n");
    uart_puts("1. All LEDs ON\r\n");
    uart_puts("2. All LEDs OFF\r\n");
    uart_puts("3. Toggle LEDs\r\n");
    uart_puts("4. Binary count\r\n");
    uart_puts("Select (1-4): ");
}

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

void main(void)
{
    unsigned char choice;
    unsigned char count = 0;
    unsigned char i;

    uart_init();
    P1 = 0xFF;  /* All OFF */

    while (1) {
        show_menu();
        choice = uart_rx();
        uart_tx(choice);  /* Echo */
        uart_puts("\r\n");

        switch (choice) {
            case '1':
                P1 = 0x00;
                uart_puts("LEDs ON\r\n");
                break;
            case '2':
                P1 = 0xFF;
                uart_puts("LEDs OFF\r\n");
                break;
            case '3':
                P1 = ~P1;
                uart_puts("LEDs Toggled\r\n");
                break;
            case '4':
                uart_puts("Counting...\r\n");
                for (i = 0; i < 16; i++) {
                    P1 = ~i;
                    delay_ms(200);
                }
                uart_puts("Done!\r\n");
                break;
            default:
                uart_puts("Invalid choice!\r\n");
        }
    }
}
