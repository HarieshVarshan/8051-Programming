/*
 * 02_voltmeter.c - Digital Voltmeter
 * Module 09: ADC & Sensors
 *
 * Description: Measure 0-5V and display via serial
 * Hardware: ADC0804, Serial connection
 */

#include <8052.h>

/* ADC0804 Control Pins */
__sbit __at (0xB5) ADC_CS;
__sbit __at (0xB6) ADC_RD;
__sbit __at (0xB7) ADC_WR;
__sbit __at (0xB2) ADC_INTR;
#define ADC_DATA P1

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

/* ADC Functions */
unsigned char adc_convert(void)
{
    unsigned char data;

    /* Start conversion */
    ADC_CS = 0;
    ADC_WR = 0;
    delay_us(1);
    ADC_WR = 1;
    ADC_CS = 1;

    /* Wait for conversion */
    while (ADC_INTR == 1);

    /* Read result */
    P1 = 0xFF;
    ADC_CS = 0;
    ADC_RD = 0;
    delay_us(1);
    data = ADC_DATA;
    ADC_RD = 1;
    ADC_CS = 1;

    return data;
}

/* Display voltage in format X.XX V */
void display_voltage(unsigned int millivolts)
{
    unsigned char volts, dec1, dec2;

    volts = millivolts / 1000;
    dec1 = (millivolts / 100) % 10;
    dec2 = (millivolts / 10) % 10;

    uart_tx('0' + volts);
    uart_tx('.');
    uart_tx('0' + dec1);
    uart_tx('0' + dec2);
    uart_puts(" V");
}

void main(void)
{
    unsigned char adc_value;
    unsigned int millivolts;

    /* Initialize */
    ADC_CS = 1;
    ADC_RD = 1;
    ADC_WR = 1;
    uart_init();

    uart_puts("Digital Voltmeter\r\n");
    uart_puts("================\r\n\r\n");

    while (1) {
        /* Read ADC */
        adc_value = adc_convert();

        /* Convert to millivolts */
        /* millivolts = (adc_value * 5000) / 256 */
        /* Simplified to avoid overflow: */
        millivolts = ((unsigned int)adc_value * 500) / 26;

        /* Display */
        uart_puts("Voltage: ");
        display_voltage(millivolts);
        uart_puts("\r\n");

        delay_ms(500);
    }
}
