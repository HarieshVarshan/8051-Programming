/*
 * lcd.h - 16x2 LCD Library (4-bit mode)
 * 8051 Bootcamp Shared Library
 *
 * Usage:
 *   1. Define LCD pins before including:
 *      #define LCD_RS P2_0
 *      #define LCD_EN P2_1
 *      #define LCD_DATA P2
 *      #include "../lib/lcd.h"
 *
 *   2. Or use defaults (P2.0=RS, P2.1=EN, P2.4-7=Data)
 *
 * LCD is connected in 4-bit mode using upper nibble of data port
 */

#ifndef LCD_H
#define LCD_H

#include <8052.h>

/* Default pin definitions (can override before include) */
#ifndef LCD_RS
__sbit __at (0xA0) LCD_RS;    /* P2.0 */
#endif

#ifndef LCD_EN
__sbit __at (0xA1) LCD_EN;    /* P2.1 */
#endif

#ifndef LCD_DATA
#define LCD_DATA P2           /* P2.4-P2.7 for data */
#endif

/* LCD Commands */
#define LCD_CLEAR       0x01
#define LCD_HOME        0x02
#define LCD_ENTRY_INC   0x06
#define LCD_DISPLAY_ON  0x0C
#define LCD_CURSOR_ON   0x0E
#define LCD_BLINK_ON    0x0F
#define LCD_FUNC_4BIT   0x28
#define LCD_LINE1       0x80
#define LCD_LINE2       0xC0

/* Internal delay functions */
static void _lcd_delay_us(unsigned int us)
{
    while (us--);
}

static void _lcd_delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

/*
 * Send 4-bit nibble to LCD
 */
static void lcd_nibble(unsigned char nibble)
{
    LCD_DATA = (LCD_DATA & 0x0F) | (nibble & 0xF0);
    LCD_EN = 1;
    _lcd_delay_us(1);
    LCD_EN = 0;
    _lcd_delay_us(50);
}

/*
 * Send command to LCD
 *
 * @param cmd: Command byte
 */
void lcd_cmd(unsigned char cmd)
{
    LCD_RS = 0;
    lcd_nibble(cmd);
    lcd_nibble(cmd << 4);

    if (cmd == LCD_CLEAR || cmd == LCD_HOME)
        _lcd_delay_ms(2);
}

/*
 * Send data (character) to LCD
 *
 * @param dat: Character to display
 */
void lcd_data(unsigned char dat)
{
    LCD_RS = 1;
    lcd_nibble(dat);
    lcd_nibble(dat << 4);
}

/*
 * Initialize LCD in 4-bit mode
 * Must be called before any other LCD functions
 */
void lcd_init(void)
{
    _lcd_delay_ms(20);

    LCD_RS = 0;
    LCD_EN = 0;

    /* Special initialization sequence */
    lcd_nibble(0x30);
    _lcd_delay_ms(5);
    lcd_nibble(0x30);
    _lcd_delay_us(150);
    lcd_nibble(0x30);
    _lcd_delay_us(150);
    lcd_nibble(0x20);
    _lcd_delay_us(150);

    /* Configure LCD */
    lcd_cmd(LCD_FUNC_4BIT);   /* 4-bit, 2 line, 5x7 */
    lcd_cmd(LCD_DISPLAY_ON);  /* Display ON, cursor OFF */
    lcd_cmd(LCD_ENTRY_INC);   /* Increment cursor */
    lcd_cmd(LCD_CLEAR);       /* Clear display */
}

/*
 * Clear LCD display
 */
void lcd_clear(void)
{
    lcd_cmd(LCD_CLEAR);
}

/*
 * Move cursor to home position
 */
void lcd_home(void)
{
    lcd_cmd(LCD_HOME);
}

/*
 * Set cursor position
 *
 * @param row: Row number (0 or 1)
 * @param col: Column number (0-15)
 */
void lcd_goto(unsigned char row, unsigned char col)
{
    unsigned char addr;
    addr = (row == 0) ? LCD_LINE1 : LCD_LINE2;
    lcd_cmd(addr + col);
}

/*
 * Display string at current position
 *
 * @param str: Null-terminated string
 */
void lcd_puts(char *str)
{
    while (*str) {
        lcd_data(*str++);
    }
}

/*
 * Display string at specified position
 *
 * @param row: Row number (0 or 1)
 * @param col: Column number (0-15)
 * @param str: Null-terminated string
 */
void lcd_puts_at(unsigned char row, unsigned char col, char *str)
{
    lcd_goto(row, col);
    lcd_puts(str);
}

/*
 * Display unsigned char as decimal
 *
 * @param num: Number to display (0-255)
 */
void lcd_putnum(unsigned char num)
{
    if (num >= 100) lcd_data('0' + num / 100);
    if (num >= 10) lcd_data('0' + (num / 10) % 10);
    lcd_data('0' + num % 10);
}

/*
 * Display unsigned int as decimal (0-65535)
 *
 * @param num: Number to display
 */
void lcd_putint(unsigned int num)
{
    char buf[6];
    signed char i = 4;

    buf[5] = '\0';
    do {
        buf[i--] = '0' + (num % 10);
        num /= 10;
    } while (num > 0 && i >= 0);

    lcd_puts(&buf[i + 1]);
}

/*
 * Display unsigned char as hex (no prefix)
 *
 * @param num: Number to display
 */
void lcd_puthex(unsigned char num)
{
    unsigned char nibble;

    nibble = (num >> 4) & 0x0F;
    lcd_data(nibble < 10 ? '0' + nibble : 'A' + nibble - 10);

    nibble = num & 0x0F;
    lcd_data(nibble < 10 ? '0' + nibble : 'A' + nibble - 10);
}

#endif /* LCD_H */
