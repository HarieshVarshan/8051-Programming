/*
 * 03_thermometer.c - Digital Thermometer
 * Module 09: ADC & Sensors
 *
 * Description: LM35 temperature sensor with serial display
 * Hardware: LM35 on ADC input, Serial connection
 * LM35: 10mV per degree Celsius
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

/* ADC Function */
unsigned char adc_convert(void)
{
    unsigned char data;

    ADC_CS = 0;
    ADC_WR = 0;
    delay_us(1);
    ADC_WR = 1;
    ADC_CS = 1;

    while (ADC_INTR == 1);

    P1 = 0xFF;
    ADC_CS = 0;
    ADC_RD = 0;
    delay_us(1);
    data = ADC_DATA;
    ADC_RD = 1;
    ADC_CS = 1;

    return data;
}

/* Display temperature */
void display_temp(unsigned int temp_x10)
{
    unsigned char tens, ones, decimal;

    tens = temp_x10 / 100;
    ones = (temp_x10 / 10) % 10;
    decimal = temp_x10 % 10;

    if (tens > 0) uart_tx('0' + tens);
    uart_tx('0' + ones);
    uart_tx('.');
    uart_tx('0' + decimal);
    uart_puts(" C");
}

void main(void)
{
    unsigned char adc_value;
    unsigned int millivolts;
    unsigned int temp_x10;  /* Temperature * 10 */

    /* Initialize */
    ADC_CS = 1;
    ADC_RD = 1;
    ADC_WR = 1;
    uart_init();

    uart_puts("LM35 Thermometer\r\n");
    uart_puts("================\r\n\r\n");

    while (1) {
        /* Read ADC */
        adc_value = adc_convert();

        /* Convert to millivolts */
        /* millivolts = (adc_value * 5000) / 256 */
        millivolts = ((unsigned int)adc_value * 500) / 26;

        /* LM35: 10mV per degree Celsius */
        /* temperature = millivolts / 10 */
        /* We keep one decimal: temp_x10 = millivolts */
        temp_x10 = millivolts / 10;

        /* Display */
        uart_puts("Temperature: ");
        display_temp(temp_x10);
        uart_puts("\r\n");

        delay_ms(1000);
    }
}
