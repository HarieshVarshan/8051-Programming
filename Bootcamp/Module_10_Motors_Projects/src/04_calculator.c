/*
 * 04_calculator.c - Simple Calculator
 * Module 10: Motors & Projects
 *
 * Description: Basic calculator with keypad and LCD
 * Hardware: 4x4 keypad on P2, LCD on P3
 * Keys: 0-9 = digits, A=+, B=-, C=*, D=/, #=equals, *=clear
 */

#include <8052.h>

/* Keypad on P2 */
#define KEYPAD_PORT P2
__sbit __at (0xA0) ROW0;
__sbit __at (0xA1) ROW1;
__sbit __at (0xA2) ROW2;
__sbit __at (0xA3) ROW3;
__sbit __at (0xA4) COL0;
__sbit __at (0xA5) COL1;
__sbit __at (0xA6) COL2;
__sbit __at (0xA7) COL3;

/* LCD on P3 */
__sbit __at (0xB0) LCD_RS;
__sbit __at (0xB1) LCD_EN;
#define LCD_DATA P3

__code char KEYMAP[4][4] = {
    {'1', '2', '3', '+'},   /* A = + */
    {'4', '5', '6', '-'},   /* B = - */
    {'7', '8', '9', '*'},   /* C = * */
    {'C', '0', '=', '/'}    /* D = /, * = C(lear), # = = */
};

/* Calculator state */
long num1 = 0, num2 = 0, result = 0;
char operator = 0;
unsigned char entering_num2 = 0;

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
    LCD_DATA = (LCD_DATA & 0x0F) | (nibble & 0xF0);
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

/* Display number on LCD */
void lcd_putnum(long num)
{
    char buf[12];
    signed char i = 10;
    unsigned char neg = 0;

    if (num < 0) { neg = 1; num = -num; }

    buf[11] = '\0';
    do {
        buf[i--] = '0' + (num % 10);
        num /= 10;
    } while (num > 0 && i >= 0);

    if (neg && i >= 0) buf[i--] = '-';

    lcd_puts(&buf[i + 1]);
}

/* Keypad Functions */
char keypad_scan(void)
{
    unsigned char row;
    char key = 0;

    for (row = 0; row < 4; row++) {
        KEYPAD_PORT = ~(1 << row);
        delay_ms(1);
        if (COL0 == 0) { key = KEYMAP[row][0]; break; }
        if (COL1 == 0) { key = KEYMAP[row][1]; break; }
        if (COL2 == 0) { key = KEYMAP[row][2]; break; }
        if (COL3 == 0) { key = KEYMAP[row][3]; break; }
    }
    KEYPAD_PORT = 0xFF;
    return key;
}

char keypad_getkey(void)
{
    char key1, key2;
    do { key1 = keypad_scan(); } while (key1 == 0);
    delay_ms(20);
    key2 = keypad_scan();
    if (key1 != key2) return 0;
    while (keypad_scan() != 0);
    delay_ms(20);
    return key1;
}

/* Calculate result */
void calculate(void)
{
    switch (operator) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/': result = (num2 != 0) ? num1 / num2 : 0; break;
        default: result = num1;
    }
}

void clear_calc(void)
{
    num1 = num2 = result = 0;
    operator = 0;
    entering_num2 = 0;
    lcd_cmd(0x01);  /* Clear LCD */
    lcd_goto(0, 0);
}

void main(void)
{
    char key;

    lcd_init();
    KEYPAD_PORT = 0xFF;

    lcd_puts("Calculator");
    delay_ms(1000);
    clear_calc();

    while (1) {
        key = keypad_getkey();

        if (key >= '0' && key <= '9') {
            /* Digit entry */
            if (!entering_num2) {
                num1 = num1 * 10 + (key - '0');
                lcd_goto(0, 0);
                lcd_puts("                ");
                lcd_goto(0, 0);
                lcd_putnum(num1);
            } else {
                num2 = num2 * 10 + (key - '0');
                lcd_goto(1, 0);
                lcd_puts("                ");
                lcd_goto(1, 0);
                lcd_putnum(num2);
            }
        }
        else if (key == '+' || key == '-' || key == '*' || key == '/') {
            /* Operator */
            operator = key;
            entering_num2 = 1;
            lcd_goto(0, 14);
            lcd_data(operator);
        }
        else if (key == '=') {
            /* Calculate */
            calculate();
            lcd_cmd(0x01);
            lcd_goto(0, 0);
            lcd_putnum(result);
            num1 = result;
            num2 = 0;
            operator = 0;
            entering_num2 = 0;
        }
        else if (key == 'C') {
            /* Clear */
            clear_calc();
        }
    }
}
