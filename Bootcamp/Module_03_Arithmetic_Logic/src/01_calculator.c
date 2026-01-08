/*
 * 01_calculator.c - Basic Arithmetic
 * Module 03: Arithmetic & Logic
 *
 * Description: Demonstrates arithmetic operations
 * Output shown on P1 (connect LEDs to see result)
 */

#include <8052.h>

void delay(void)
{
    unsigned int i, j;
    for (i = 0; i < 200; i++)
        for (j = 0; j < 500; j++);
}

void main(void)
{
    unsigned char a = 10;
    unsigned char b = 3;
    unsigned char result;

    while (1) {
        /* Addition: 10 + 3 = 13 (0x0D) */
        result = a + b;
        P1 = ~result;
        delay();

        /* Subtraction: 10 - 3 = 7 (0x07) */
        result = a - b;
        P1 = ~result;
        delay();

        /* Multiplication: 10 * 3 = 30 (0x1E) */
        result = a * b;
        P1 = ~result;
        delay();

        /* Division: 10 / 3 = 3 (0x03) */
        result = a / b;
        P1 = ~result;
        delay();

        /* Modulus: 10 % 3 = 1 (0x01) */
        result = a % b;
        P1 = ~result;
        delay();
    }
}
