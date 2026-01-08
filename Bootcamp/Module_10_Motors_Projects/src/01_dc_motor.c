/*
 * 01_dc_motor.c - DC Motor Control
 * Module 10: Motors & Projects
 *
 * Description: PWM speed control for DC motor via L293D
 * Hardware: L293D on P1, buttons on P3.2, P3.3
 *           P1.0=EN, P1.1=IN1, P1.2=IN2
 */

#include <8052.h>

/* Motor control pins */
__sbit __at (0x90) MOTOR_EN;   /* P1.0 - Enable (PWM) */
__sbit __at (0x91) MOTOR_IN1;  /* P1.1 - Direction 1 */
__sbit __at (0x92) MOTOR_IN2;  /* P1.2 - Direction 2 */

/* Button inputs */
__sbit __at (0xB2) BTN_SPEED;  /* P3.2 - Speed button */
__sbit __at (0xB3) BTN_DIR;    /* P3.3 - Direction button */

/* PWM variables */
unsigned char pwm_duty = 50;   /* 0-100% */
unsigned char pwm_counter = 0;
unsigned char direction = 1;   /* 1=forward, 0=reverse */

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

/* Set motor direction */
void motor_set_direction(unsigned char dir)
{
    if (dir) {
        MOTOR_IN1 = 1;
        MOTOR_IN2 = 0;
    } else {
        MOTOR_IN1 = 0;
        MOTOR_IN2 = 1;
    }
}

/* Stop motor */
void motor_stop(void)
{
    MOTOR_EN = 0;
    MOTOR_IN1 = 0;
    MOTOR_IN2 = 0;
}

/* Software PWM - call in tight loop */
void pwm_update(void)
{
    pwm_counter++;
    if (pwm_counter >= 100) pwm_counter = 0;

    if (pwm_counter < pwm_duty) {
        MOTOR_EN = 1;
    } else {
        MOTOR_EN = 0;
    }
}

void main(void)
{
    unsigned int i;
    unsigned char speed_pressed = 0;
    unsigned char dir_pressed = 0;

    /* Initialize */
    motor_stop();
    motor_set_direction(direction);
    pwm_duty = 50;

    while (1) {
        /* PWM loop - run many times for smooth PWM */
        for (i = 0; i < 100; i++) {
            pwm_update();
        }

        /* Check speed button */
        if (BTN_SPEED == 0 && !speed_pressed) {
            speed_pressed = 1;
            delay_ms(20);  /* Debounce */

            /* Cycle through speeds: 25, 50, 75, 100 */
            pwm_duty += 25;
            if (pwm_duty > 100) pwm_duty = 25;
        }
        if (BTN_SPEED == 1) speed_pressed = 0;

        /* Check direction button */
        if (BTN_DIR == 0 && !dir_pressed) {
            dir_pressed = 1;
            delay_ms(20);  /* Debounce */

            /* Toggle direction */
            direction = !direction;
            motor_set_direction(direction);
        }
        if (BTN_DIR == 1) dir_pressed = 0;
    }
}
