/*
 * 01_adc_read.c - Basic ADC Reading
 * Module 09: ADC & Sensors
 *
 * Description: Read ADC0804 and display on LEDs
 * Hardware: ADC0804 data on P1, control on P3
 *           Potentiometer on ADC input
 */

#include <8052.h>

/* ADC0804 Control Pins */
__sbit __at (0xB5) ADC_CS;    /* P3.5 - Chip Select */
__sbit __at (0xB6) ADC_RD;    /* P3.6 - Read */
__sbit __at (0xB7) ADC_WR;    /* P3.7 - Write */
__sbit __at (0xB2) ADC_INTR;  /* P3.2 - Interrupt (conversion done) */

#define ADC_DATA P1           /* Data bus */

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

/* Start ADC conversion */
void adc_start(void)
{
    ADC_CS = 0;     /* Select ADC */
    ADC_WR = 0;     /* Start conversion */
    delay_us(1);
    ADC_WR = 1;     /* WR pulse complete */
    ADC_CS = 1;
}

/* Wait for conversion complete */
void adc_wait(void)
{
    while (ADC_INTR == 1);  /* Wait for INTR to go low */
}

/* Read ADC result */
unsigned char adc_read(void)
{
    unsigned char data;

    P1 = 0xFF;      /* Configure P1 as input */
    ADC_CS = 0;     /* Select ADC */
    ADC_RD = 0;     /* Enable output */
    delay_us(1);
    data = ADC_DATA; /* Read data */
    ADC_RD = 1;     /* Disable output */
    ADC_CS = 1;

    return data;
}

/* Complete ADC conversion */
unsigned char adc_convert(void)
{
    adc_start();
    adc_wait();
    return adc_read();
}

void main(void)
{
    unsigned char adc_value;

    /* Initialize control pins */
    ADC_CS = 1;
    ADC_RD = 1;
    ADC_WR = 1;

    while (1) {
        /* Read ADC */
        adc_value = adc_convert();

        /* Display on LEDs (directly) */
        P1 = ~adc_value;  /* Inverted for active-low LEDs */

        delay_ms(100);
    }
}
