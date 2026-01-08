/*
 * 03_pwm_timer.c - Timer-based PWM
 * Module 05: Timers
 *
 * Description: PWM using Timer for LED dimming
 * Hardware: LED on P1.0
 */

#include <8052.h>

__sbit __at (0x90) PWM_OUT;

unsigned char duty_cycle = 50;  /* 0-100% */

void timer0_init(void)
{
    TMOD = 0x02;    /* Timer 0, Mode 2 (8-bit auto-reload) */
    TH0 = 0x00;     /* Reload value for ~256µs period */
    TL0 = 0x00;
    TR0 = 1;        /* Start timer */
}

void main(void)
{
    unsigned char pwm_count = 0;

    timer0_init();

    while (1) {
        if (TF0) {
            TF0 = 0;
            pwm_count++;

            if (pwm_count >= 100) {
                pwm_count = 0;
            }

            /* Set output based on duty cycle */
            if (pwm_count < duty_cycle) {
                PWM_OUT = 0;  /* ON */
            } else {
                PWM_OUT = 1;  /* OFF */
            }
        }

        /* Slowly change duty cycle for demo */
        /* In real use, this would be controlled externally */
    }
}
