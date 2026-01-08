/*
 * 05_temp_controller.c - Temperature Controller
 * Module 10: Motors & Projects
 *
 * Description: Temperature controller with setpoint and relay output
 * Hardware: LM35 on ADC, LCD display, relay on P1.7
 *           Buttons: P3.2 (up), P3.3 (down)
 */

#include <8052.h>

/* ADC0804 Control Pins */
__sbit __at (0xB5) ADC_CS;
__sbit __at (0xB6) ADC_RD;
__sbit __at (0xB7) ADC_WR;
__sbit __at (0xB2) ADC_INTR;
#define ADC_DATA P1

/* Output relay/heater */
__sbit __at (0x97) RELAY;     /* P1.7 */

/* Setpoint buttons */
__sbit __at (0xB2) BTN_UP;    /* Note: shared with ADC_INTR */
__sbit __at (0xB3) BTN_DOWN;

/* LCD on P2 */
__sbit __at (0xA0) LCD_RS;
__sbit __at (0xA1) LCD_EN;
#define LCD_PORT P2

/* Controller settings */
unsigned char setpoint = 25;  /* Target temperature */
unsigned char hysteresis = 2; /* +/- deadband */
unsigned char current_temp = 0;
unsigned char relay_state = 0;

void delay_us(unsigned int us) { while (us--); }
void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

/* LCD Functions */
void lcd_nibble(unsigned char nibble)
{
    LCD_PORT = (LCD_PORT & 0x0F) | (nibble & 0xF0);
    LCD_EN = 1; delay_us(1); LCD_EN = 0; delay_us(50);
}

void lcd_cmd(unsigned char cmd)
{
    LCD_RS = 0;
    lcd_nibble(cmd);
    lcd_nibble(cmd << 4);
    if (cmd == 0x01) delay_ms(2);
}

void lcd_data(unsigned char dat)
{
    LCD_RS = 1;
    lcd_nibble(dat);
    lcd_nibble(dat << 4);
}

void lcd_puts(char *str) { while (*str) lcd_data(*str++); }

void lcd_goto(unsigned char row, unsigned char col)
{
    lcd_cmd((row == 0 ? 0x80 : 0xC0) + col);
}

void lcd_init(void)
{
    delay_ms(20);
    LCD_RS = 0; LCD_EN = 0;
    lcd_nibble(0x30); delay_ms(5);
    lcd_nibble(0x30); delay_us(150);
    lcd_nibble(0x30); delay_us(150);
    lcd_nibble(0x20); delay_us(150);
    lcd_cmd(0x28); lcd_cmd(0x0C);
    lcd_cmd(0x06); lcd_cmd(0x01);
}

void lcd_putnum(unsigned char num)
{
    lcd_data('0' + (num / 10));
    lcd_data('0' + (num % 10));
}

/* ADC Function */
unsigned char adc_convert(void)
{
    unsigned char data;

    /* Start conversion */
    P1 = 0xFF;  /* Set P1 as input temporarily */
    ADC_CS = 0;
    ADC_WR = 0;
    delay_us(1);
    ADC_WR = 1;
    ADC_CS = 1;

    /* Wait for conversion (poll INTR or use delay) */
    delay_ms(1);  /* ~100us conversion time */

    /* Read result */
    ADC_CS = 0;
    ADC_RD = 0;
    delay_us(1);
    data = ADC_DATA;
    ADC_RD = 1;
    ADC_CS = 1;

    return data;
}

/* Convert ADC to temperature (LM35: 10mV/C) */
unsigned char read_temperature(void)
{
    unsigned char adc_value;
    unsigned int millivolts;

    adc_value = adc_convert();

    /* millivolts = (adc * 5000) / 256 */
    millivolts = ((unsigned int)adc_value * 500) / 26;

    /* LM35: 10mV per degree */
    return millivolts / 10;
}

/* Update display */
void update_display(void)
{
    lcd_goto(0, 0);
    lcd_puts("Temp: ");
    lcd_putnum(current_temp);
    lcd_puts("C  ");

    lcd_goto(1, 0);
    lcd_puts("Set:  ");
    lcd_putnum(setpoint);
    lcd_puts("C ");

    /* Show relay status */
    lcd_goto(1, 12);
    if (relay_state) {
        lcd_puts("HEAT");
    } else {
        lcd_puts(" OFF");
    }
}

/* Temperature control with hysteresis */
void control_temperature(void)
{
    if (current_temp < (setpoint - hysteresis)) {
        /* Below setpoint - turn on heating */
        relay_state = 1;
        RELAY = 1;
    }
    else if (current_temp > (setpoint + hysteresis)) {
        /* Above setpoint - turn off heating */
        relay_state = 0;
        RELAY = 0;
    }
    /* Within deadband - maintain current state */
}

void main(void)
{
    unsigned char btn_up_prev = 1;
    unsigned char btn_down_prev = 1;

    /* Initialize */
    lcd_init();
    ADC_CS = 1;
    ADC_RD = 1;
    ADC_WR = 1;
    RELAY = 0;

    lcd_puts("Temp Controller");
    delay_ms(1000);
    lcd_cmd(0x01);

    while (1) {
        /* Read temperature */
        current_temp = read_temperature();

        /* Control relay */
        control_temperature();

        /* Update display */
        update_display();

        /* Check setpoint buttons */
        if (BTN_UP == 0 && btn_up_prev == 1) {
            delay_ms(20);  /* Debounce */
            if (setpoint < 99) setpoint++;
        }
        btn_up_prev = BTN_UP;

        if (BTN_DOWN == 0 && btn_down_prev == 1) {
            delay_ms(20);  /* Debounce */
            if (setpoint > 0) setpoint--;
        }
        btn_down_prev = BTN_DOWN;

        delay_ms(200);  /* Update rate */
    }
}
