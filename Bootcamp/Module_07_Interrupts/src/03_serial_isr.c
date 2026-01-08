/*
 * 03_serial_isr.c - Interrupt-Driven Serial
 * Module 07: Interrupts
 *
 * Description: Non-blocking serial echo using interrupts
 * Hardware: Serial connection to PC
 * Baud: 9600, 8N1
 */

#include <8052.h>

#define BUFFER_SIZE 16

/* Circular receive buffer */
volatile unsigned char rx_buffer[BUFFER_SIZE];
volatile unsigned char rx_head = 0;
volatile unsigned char rx_tail = 0;
volatile unsigned char rx_count = 0;

/* Transmit buffer */
volatile unsigned char tx_buffer[BUFFER_SIZE];
volatile unsigned char tx_head = 0;
volatile unsigned char tx_tail = 0;
volatile unsigned char tx_busy = 0;

/* Serial ISR */
void serial_isr(void) __interrupt(4)
{
    if (RI) {
        RI = 0;  /* Clear receive flag */

        /* Store in buffer if not full */
        if (rx_count < BUFFER_SIZE) {
            rx_buffer[rx_head] = SBUF;
            rx_head = (rx_head + 1) % BUFFER_SIZE;
            rx_count++;
        }
    }

    if (TI) {
        TI = 0;  /* Clear transmit flag */

        /* Send next byte if available */
        if (tx_head != tx_tail) {
            SBUF = tx_buffer[tx_tail];
            tx_tail = (tx_tail + 1) % BUFFER_SIZE;
        } else {
            tx_busy = 0;  /* No more data to send */
        }
    }
}

void uart_init(void)
{
    TMOD = 0x20;    /* Timer 1, Mode 2 */
    TH1 = 0xFD;     /* 9600 baud */
    SCON = 0x50;    /* Mode 1, REN enabled */
    TR1 = 1;        /* Start Timer 1 */
    ES = 1;         /* Enable Serial interrupt */
    EA = 1;         /* Global interrupt enable */
}

/* Check if data available */
unsigned char uart_available(void)
{
    return rx_count;
}

/* Read byte from buffer (non-blocking) */
unsigned char uart_read(void)
{
    unsigned char data;

    if (rx_count == 0) return 0;

    EA = 0;  /* Disable interrupts briefly */
    data = rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) % BUFFER_SIZE;
    rx_count--;
    EA = 1;  /* Re-enable interrupts */

    return data;
}

/* Queue byte for transmission */
void uart_write(unsigned char c)
{
    /* Wait if buffer full */
    while ((tx_head + 1) % BUFFER_SIZE == tx_tail);

    EA = 0;  /* Disable interrupts briefly */
    tx_buffer[tx_head] = c;
    tx_head = (tx_head + 1) % BUFFER_SIZE;

    /* Start transmission if idle */
    if (!tx_busy) {
        tx_busy = 1;
        SBUF = tx_buffer[tx_tail];
        tx_tail = (tx_tail + 1) % BUFFER_SIZE;
    }
    EA = 1;
}

/* Send string */
void uart_puts(char *str)
{
    while (*str) uart_write(*str++);
}

void main(void)
{
    unsigned char c;

    uart_init();
    uart_puts("Interrupt Serial Echo\r\n");
    uart_puts("Type something:\r\n");

    while (1) {
        /* Check for received data */
        if (uart_available()) {
            c = uart_read();
            uart_write(c);  /* Echo back */

            if (c == '\r') {
                uart_write('\n');
            }
        }

        /* Main loop can do other work here */
        /* Serial communication is non-blocking */
    }
}
