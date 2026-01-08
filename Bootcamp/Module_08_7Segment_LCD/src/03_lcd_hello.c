/*
 * 03_lcd_hello.c - LCD Hello World
 * Module 08: 7-Segment & LCD
 *
 * Description: Display "Hello World!" on 16x2 LCD
 * Hardware: LCD in 4-bit mode on P2
 *           RS=P2.0, EN=P2.1, D4-D7=P2.4-P2.7
 */

#include <8052.h>

/* LCD Pin Definitions */
__sbit __at (0xA0) LCD_RS;    /* P2.0 - Register Select */
__sbit __at (0xA1) LCD_EN;    /* P2.1 - Enable */
#define LCD_DATA P2           /* P2.4-P2.7 for data */

/* LCD Commands */
#define LCD_CLEAR       0x01
#define LCD_HOME        0x02
#define LCD_ENTRY_MODE  0x06
#define LCD_DISPLAY_ON  0x0C
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

/* Send 4-bit nibble to LCD */
void lcd_nibble(unsigned char nibble)
{
    /* Put high nibble on P2.4-P2.7 */
    LCD_DATA = (LCD_DATA & 0x0F) | (nibble & 0xF0);

    /* Pulse enable */
    LCD_EN = 1;
    delay_us(1);
    LCD_EN = 0;
    delay_us(50);
}

/* Send command to LCD */
void lcd_cmd(unsigned char cmd)
{
    LCD_RS = 0;                 /* Command mode */
    lcd_nibble(cmd);            /* High nibble */
    lcd_nibble(cmd << 4);       /* Low nibble */

    if (cmd == LCD_CLEAR || cmd == LCD_HOME)
        delay_ms(2);            /* Clear/Home need more time */
}

/* Send data to LCD */
void lcd_data(unsigned char dat)
{
    LCD_RS = 1;                 /* Data mode */
    lcd_nibble(dat);            /* High nibble */
    lcd_nibble(dat << 4);       /* Low nibble */
}

/* Send string to LCD */
void lcd_puts(char *str)
{
    while (*str) {
        lcd_data(*str++);
    }
}

/* Set cursor position */
void lcd_goto(unsigned char row, unsigned char col)
{
    unsigned char addr;
    addr = (row == 0) ? LCD_LINE1 : LCD_LINE2;
    addr += col;
    lcd_cmd(addr);
}

/* Initialize LCD in 4-bit mode */
void lcd_init(void)
{
    delay_ms(20);               /* Wait for LCD power up */

    LCD_RS = 0;
    LCD_EN = 0;

    /* Special initialization sequence for 4-bit mode */
    lcd_nibble(0x30);           /* Function set 8-bit */
    delay_ms(5);
    lcd_nibble(0x30);           /* Function set 8-bit */
    delay_us(150);
    lcd_nibble(0x30);           /* Function set 8-bit */
    delay_us(150);
    lcd_nibble(0x20);           /* Function set 4-bit */
    delay_us(150);

    /* Now in 4-bit mode, configure LCD */
    lcd_cmd(LCD_FUNC_4BIT);     /* 4-bit, 2 line, 5x7 */
    lcd_cmd(LCD_DISPLAY_ON);    /* Display ON, cursor OFF */
    lcd_cmd(LCD_ENTRY_MODE);    /* Increment cursor */
    lcd_cmd(LCD_CLEAR);         /* Clear display */
}

void main(void)
{
    lcd_init();

    /* Display on line 1 */
    lcd_goto(0, 0);
    lcd_puts("Hello World!");

    /* Display on line 2 */
    lcd_goto(1, 0);
    lcd_puts("8051 LCD Demo");

    while (1) {
        /* Static display - nothing to do */
    }
}
