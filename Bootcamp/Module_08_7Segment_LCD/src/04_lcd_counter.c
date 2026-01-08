/*
 * 04_lcd_counter.c - LCD Running Counter
 * Module 08: 7-Segment & LCD
 *
 * Description: Display incrementing counter on LCD
 * Hardware: LCD in 4-bit mode on P2
 */

#include <8052.h>

/* LCD Pin Definitions */
__sbit __at (0xA0) LCD_RS;    /* P2.0 */
__sbit __at (0xA1) LCD_EN;    /* P2.1 */
#define LCD_DATA P2

/* LCD Commands */
#define LCD_CLEAR       0x01
#define LCD_DISPLAY_ON  0x0C
#define LCD_ENTRY_MODE  0x06
#define LCD_FUNC_4BIT   0x28
#define LCD_LINE1       0x80
#define LCD_LINE2       0xC0

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

void lcd_nibble(unsigned char nibble)
{
    LCD_DATA = (LCD_DATA & 0x0F) | (nibble & 0xF0);
    LCD_EN = 1;
    delay_us(1);
    LCD_EN = 0;
    delay_us(50);
}

void lcd_cmd(unsigned char cmd)
{
    LCD_RS = 0;
    lcd_nibble(cmd);
    lcd_nibble(cmd << 4);
    if (cmd == LCD_CLEAR) delay_ms(2);
}

void lcd_data(unsigned char dat)
{
    LCD_RS = 1;
    lcd_nibble(dat);
    lcd_nibble(dat << 4);
}

void lcd_puts(char *str)
{
    while (*str) lcd_data(*str++);
}

void lcd_goto(unsigned char row, unsigned char col)
{
    unsigned char addr;
    addr = (row == 0) ? LCD_LINE1 : LCD_LINE2;
    lcd_cmd(addr + col);
}

void lcd_init(void)
{
    delay_ms(20);
    LCD_RS = 0;
    LCD_EN = 0;

    lcd_nibble(0x30);
    delay_ms(5);
    lcd_nibble(0x30);
    delay_us(150);
    lcd_nibble(0x30);
    delay_us(150);
    lcd_nibble(0x20);
    delay_us(150);

    lcd_cmd(LCD_FUNC_4BIT);
    lcd_cmd(LCD_DISPLAY_ON);
    lcd_cmd(LCD_ENTRY_MODE);
    lcd_cmd(LCD_CLEAR);
}

/* Display unsigned int on LCD */
void lcd_putnum(unsigned int num)
{
    char buf[6];
    signed char i = 4;

    /* Convert to string (reverse) */
    buf[5] = '\0';
    do {
        buf[i--] = '0' + (num % 10);
        num /= 10;
    } while (num > 0 && i >= 0);

    /* Pad with spaces */
    while (i >= 0) buf[i--] = ' ';

    lcd_puts(buf);
}

void main(void)
{
    unsigned int counter = 0;

    lcd_init();

    lcd_goto(0, 0);
    lcd_puts("8051 Counter");

    lcd_goto(1, 0);
    lcd_puts("Value: ");

    while (1) {
        /* Update counter display */
        lcd_goto(1, 7);
        lcd_putnum(counter);

        delay_ms(100);

        counter++;
        if (counter > 65535) counter = 0;
    }
}
