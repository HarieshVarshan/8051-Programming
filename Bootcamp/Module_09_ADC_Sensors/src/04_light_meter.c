/*
 * 04_light_meter.c - Light Intensity Meter
 * Module 09: ADC & Sensors
 *
 * Description: LDR light sensor with LED bar graph
 * Hardware: LDR voltage divider on ADC, LEDs on P2
 */

#include <8052.h>

/* ADC0804 Control Pins */
__sbit __at (0xB5) ADC_CS;
__sbit __at (0xB6) ADC_RD;
__sbit __at (0xB7) ADC_WR;
__sbit __at (0xB2) ADC_INTR;
#define ADC_DATA P1
#define LED_BAR P2

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

/* Convert level (0-8) to bar graph pattern */
unsigned char level_to_bar(unsigned char level)
{
    /* Bar graph patterns (active low LEDs) */
    __code unsigned char bars[] = {
        0xFF,   /* Level 0: -------- */
        0xFE,   /* Level 1: *------- */
        0xFC,   /* Level 2: **------ */
        0xF8,   /* Level 3: ***----- */
        0xF0,   /* Level 4: ****---- */
        0xE0,   /* Level 5: *****--- */
        0xC0,   /* Level 6: ******-- */
        0x80,   /* Level 7: *******- */
        0x00    /* Level 8: ******** */
    };

    if (level > 8) level = 8;
    return bars[level];
}

void main(void)
{
    unsigned char adc_value;
    unsigned char light_level;

    /* Initialize */
    ADC_CS = 1;
    ADC_RD = 1;
    ADC_WR = 1;
    LED_BAR = 0xFF;  /* All LEDs off */

    while (1) {
        /* Read ADC (LDR value) */
        adc_value = adc_convert();

        /* Convert to 0-8 level */
        /* Higher ADC = brighter light (LDR low resistance) */
        light_level = adc_value / 32;  /* 256/8 = 32 */

        /* Update LED bar graph */
        LED_BAR = level_to_bar(light_level);

        delay_ms(50);
    }
}
