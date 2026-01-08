/*
 * 02_stepper.c - Stepper Motor Driver
 * Module 10: Motors & Projects
 *
 * Description: Control stepper motor with variable speed
 * Hardware: ULN2003 driver on P1.0-P1.3
 *           Speed pot on P3.0 (or fixed delay)
 *           Direction button on P3.2
 */

#include <8052.h>

/* Stepper coils on P1.0-P1.3 */
#define STEPPER_PORT P1

/* Direction button */
__sbit __at (0xB2) BTN_DIR;

/* Step sequences */

/* Full step - Wave drive (one coil at a time) */
__code unsigned char WAVE_STEP[] = {
    0x01,   /* Step 1: A    */
    0x02,   /* Step 2: B    */
    0x04,   /* Step 3: A'   */
    0x08    /* Step 4: B'   */
};

/* Full step - Two phase (two coils at a time, more torque) */
__code unsigned char FULL_STEP[] = {
    0x03,   /* Step 1: A  + B  */
    0x06,   /* Step 2: B  + A' */
    0x0C,   /* Step 3: A' + B' */
    0x09    /* Step 4: B' + A  */
};

/* Half step (8 steps, smoother) */
__code unsigned char HALF_STEP[] = {
    0x01,   /* Step 1: A       */
    0x03,   /* Step 2: A + B   */
    0x02,   /* Step 3: B       */
    0x06,   /* Step 4: B + A'  */
    0x04,   /* Step 5: A'      */
    0x0C,   /* Step 6: A' + B' */
    0x08,   /* Step 7: B'      */
    0x09    /* Step 8: B' + A  */
};

/* Current step position */
unsigned char step_index = 0;
unsigned char direction = 1;  /* 1=CW, 0=CCW */
unsigned char step_mode = 0;  /* 0=full, 1=half */

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

/* Take one step */
void stepper_step(void)
{
    unsigned char pattern;
    unsigned char max_steps;

    if (step_mode == 0) {
        /* Full step mode */
        max_steps = 4;
        pattern = FULL_STEP[step_index];
    } else {
        /* Half step mode */
        max_steps = 8;
        pattern = HALF_STEP[step_index];
    }

    /* Output to stepper */
    STEPPER_PORT = (STEPPER_PORT & 0xF0) | pattern;

    /* Update step index */
    if (direction) {
        step_index++;
        if (step_index >= max_steps) step_index = 0;
    } else {
        if (step_index == 0) step_index = max_steps;
        step_index--;
    }
}

/* Rotate specified number of steps */
void stepper_rotate(unsigned int steps, unsigned int delay)
{
    unsigned int i;
    for (i = 0; i < steps; i++) {
        stepper_step();
        delay_ms(delay);
    }
}

void main(void)
{
    unsigned char btn_pressed = 0;
    unsigned int step_delay = 10;  /* ms between steps */

    /* Initialize */
    STEPPER_PORT = 0x00;
    direction = 1;
    step_mode = 0;  /* Full step */

    while (1) {
        /* Take one step */
        stepper_step();
        delay_ms(step_delay);

        /* Check direction button */
        if (BTN_DIR == 0 && !btn_pressed) {
            btn_pressed = 1;
            delay_ms(20);  /* Debounce */
            direction = !direction;
        }
        if (BTN_DIR == 1) btn_pressed = 0;
    }
}
