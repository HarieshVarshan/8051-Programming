/*
 * 03_switch_input.c - Switch Input Control
 * Module 02: I/O Port Programming
 *
 * Description: Read switch state, control LED
 * Hardware: LED on P1.0, Switch on P1.7
 */

#include <8052.h>

__sbit __at (0x90) LED;      /* P1.0 - Output */
__sbit __at (0x97) SWITCH;   /* P1.7 - Input */

void main(void)
{
    /* Configure P1.7 as input by writing 1 */
    SWITCH = 1;

    while (1) {
        if (SWITCH == 0) {
            /* Switch pressed (active low) */
            LED = 0;    /* Turn LED ON */
        } else {
            /* Switch not pressed */
            LED = 1;    /* Turn LED OFF */
        }
    }
}
