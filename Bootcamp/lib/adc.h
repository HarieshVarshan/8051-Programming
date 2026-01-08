/*
 * adc.h - ADC0804 Library
 * 8051 Bootcamp Shared Library
 *
 * Usage:
 *   1. Define ADC pins before including:
 *      #define ADC_CS P3_5
 *      #define ADC_RD P3_6
 *      #define ADC_WR P3_7
 *      #define ADC_INTR P3_2
 *      #define ADC_DATA P1
 *      #include "../lib/adc.h"
 *
 *   2. Or use defaults (P3.5=CS, P3.6=RD, P3.7=WR, P3.2=INTR, P1=DATA)
 */

#ifndef ADC_H
#define ADC_H

#include <8052.h>

/* Default pin definitions */
#ifndef ADC_CS
__sbit __at (0xB5) ADC_CS;    /* P3.5 */
#endif

#ifndef ADC_RD
__sbit __at (0xB6) ADC_RD;    /* P3.6 */
#endif

#ifndef ADC_WR
__sbit __at (0xB7) ADC_WR;    /* P3.7 */
#endif

#ifndef ADC_INTR
__sbit __at (0xB2) ADC_INTR;  /* P3.2 */
#endif

#ifndef ADC_DATA
#define ADC_DATA P1
#endif

/* Internal delay */
static void _adc_delay(void)
{
    unsigned char i;
    for (i = 0; i < 10; i++);
}

/*
 * Initialize ADC pins
 * Call once at startup
 */
void adc_init(void)
{
    ADC_CS = 1;
    ADC_RD = 1;
    ADC_WR = 1;
}

/*
 * Start ADC conversion
 * Does not wait for completion
 */
void adc_start(void)
{
    ADC_CS = 0;
    ADC_WR = 0;
    _adc_delay();
    ADC_WR = 1;
    ADC_CS = 1;
}

/*
 * Wait for conversion to complete
 * Blocks until INTR goes low
 */
void adc_wait(void)
{
    while (ADC_INTR == 1);
}

/*
 * Read ADC result
 *
 * @return: 8-bit ADC value
 */
unsigned char adc_read(void)
{
    unsigned char data;

    ADC_DATA = 0xFF;  /* Configure as input */
    ADC_CS = 0;
    ADC_RD = 0;
    _adc_delay();
    data = ADC_DATA;
    ADC_RD = 1;
    ADC_CS = 1;

    return data;
}

/*
 * Complete ADC conversion (start, wait, read)
 *
 * @return: 8-bit ADC value
 */
unsigned char adc_convert(void)
{
    adc_start();
    adc_wait();
    return adc_read();
}

/*
 * Convert ADC value to millivolts (0-5000mV)
 *
 * @param adc_val: 8-bit ADC reading
 * @return: Voltage in millivolts
 */
unsigned int adc_to_mv(unsigned char adc_val)
{
    /* millivolts = (adc_val * 5000) / 256 */
    /* Simplified to avoid overflow */
    return ((unsigned int)adc_val * 500) / 26;
}

/*
 * Convert ADC to temperature (LM35: 10mV/C)
 *
 * @param adc_val: 8-bit ADC reading
 * @return: Temperature in degrees Celsius
 */
unsigned char adc_to_temp_lm35(unsigned char adc_val)
{
    unsigned int mv = adc_to_mv(adc_val);
    return mv / 10;
}

/*
 * Convert ADC to percentage (0-100%)
 *
 * @param adc_val: 8-bit ADC reading
 * @return: Percentage (0-100)
 */
unsigned char adc_to_percent(unsigned char adc_val)
{
    return ((unsigned int)adc_val * 100) / 255;
}

#endif /* ADC_H */
