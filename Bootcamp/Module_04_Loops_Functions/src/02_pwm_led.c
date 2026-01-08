/*
 * 02_pwm_led.c - Software PWM
 * Module 04: Loops & Functions
 *
 * Description: LED brightness control using PWM
 * Hardware: LED on P1.0
 */

#include <8052.h>

__sbit __at (0x90) LED;

#define PWM_PERIOD 100  /* PWM period in units */

void pwm_cycle(unsigned char duty)
{
    unsigned char i;

    /* ON time */
    LED = 0;
    for (i = 0; i < duty; i++);

    /* OFF time */
    LED = 1;
    for (i = 0; i < (PWM_PERIOD - duty); i++);
}

void main(void)
{
    unsigned char brightness = 0;
    unsigned char direction = 0;
    unsigned int cycle;

    while (1) {
        /* Run PWM for some cycles at current brightness */
        for (cycle = 0; cycle < 50; cycle++) {
            pwm_cycle(brightness);
        }

        /* Change brightness */
        if (direction == 0) {
            brightness += 5;
            if (brightness >= 95) direction = 1;
        } else {
            brightness -= 5;
            if (brightness <= 5) direction = 0;
        }
    }
}
