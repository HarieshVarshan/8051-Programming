/*
 * Password Lock System
 * Digital lock with keypad, LCD, and relay control
 *
 * Hardware:
 *   - 4x4 Keypad on P1
 *   - LCD (4-bit) on P2
 *   - Relay on P3.0, Buzzer on P3.1
 *   - LEDs: Green P3.2, Red P3.3
 */

#include <8052.h>

/* ========== Pin Definitions ========== */

/* Keypad on P1 */
#define KEYPAD_PORT P1

/* LCD on P2 */
__sbit __at (0xA0) LCD_RS;
__sbit __at (0xA1) LCD_EN;
#define LCD_DATA P2

/* Outputs on P3 */
__sbit __at (0xB0) RELAY;
__sbit __at (0xB1) BUZZER;
__sbit __at (0xB2) LED_GREEN;
__sbit __at (0xB3) LED_RED;

/* ========== Constants ========== */

#define PASSWORD_LEN    4
#define MAX_ATTEMPTS    3
#define LOCKOUT_TIME    30   /* seconds */
#define UNLOCK_TIMEOUT  60   /* seconds */

/* States */
#define STATE_LOCKED    0
#define STATE_UNLOCKED  1
#define STATE_LOCKOUT   2
#define STATE_CHANGE    3

/* Keypad layout */
__code char KEYMAP[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

/* ========== Global Variables ========== */

char password[PASSWORD_LEN + 1] = "1234";  /* Default password */
char input[PASSWORD_LEN + 1];
unsigned char input_pos = 0;
unsigned char current_state = STATE_LOCKED;
unsigned char wrong_attempts = 0;
unsigned int lockout_counter = 0;
unsigned int unlock_counter = 0;

/* ========== Delay Functions ========== */

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

/* ========== LCD Functions ========== */

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
    if (cmd == 0x01 || cmd == 0x02) delay_ms(2);
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
    lcd_cmd((row == 0 ? 0x80 : 0xC0) + col);
}

void lcd_clear(void)
{
    lcd_cmd(0x01);
}

void lcd_init(void)
{
    delay_ms(20);
    LCD_RS = 0;
    LCD_EN = 0;
    lcd_nibble(0x30); delay_ms(5);
    lcd_nibble(0x30); delay_us(150);
    lcd_nibble(0x30); delay_us(150);
    lcd_nibble(0x20); delay_us(150);
    lcd_cmd(0x28);  /* 4-bit, 2 line */
    lcd_cmd(0x0C);  /* Display on, cursor off */
    lcd_cmd(0x06);  /* Increment */
    lcd_cmd(0x01);  /* Clear */
}

void lcd_putnum(unsigned char num)
{
    if (num >= 10) lcd_data('0' + num / 10);
    lcd_data('0' + num % 10);
}

/* ========== Keypad Functions ========== */

char keypad_scan(void)
{
    unsigned char row;
    char key = 0;

    for (row = 0; row < 4; row++) {
        KEYPAD_PORT = ~(1 << row);
        delay_us(10);

        if ((KEYPAD_PORT & 0x10) == 0) { key = KEYMAP[row][0]; break; }
        if ((KEYPAD_PORT & 0x20) == 0) { key = KEYMAP[row][1]; break; }
        if ((KEYPAD_PORT & 0x40) == 0) { key = KEYMAP[row][2]; break; }
        if ((KEYPAD_PORT & 0x80) == 0) { key = KEYMAP[row][3]; break; }
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

char keypad_getkey_timeout(unsigned int timeout_ms)
{
    char key;
    unsigned int elapsed = 0;

    while (elapsed < timeout_ms) {
        key = keypad_scan();
        if (key != 0) {
            delay_ms(20);
            if (keypad_scan() == key) {
                while (keypad_scan() != 0);
                return key;
            }
        }
        delay_ms(10);
        elapsed += 10;
    }

    return 0;  /* Timeout */
}

/* ========== Sound Functions ========== */

void beep_short(void)
{
    BUZZER = 1;
    delay_ms(50);
    BUZZER = 0;
}

void beep_success(void)
{
    BUZZER = 1; delay_ms(100);
    BUZZER = 0; delay_ms(50);
    BUZZER = 1; delay_ms(100);
    BUZZER = 0;
}

void beep_error(void)
{
    BUZZER = 1; delay_ms(500);
    BUZZER = 0;
}

/* ========== Lock Functions ========== */

void lock_open(void)
{
    RELAY = 1;
    LED_GREEN = 0;  /* ON (active low) */
    LED_RED = 1;    /* OFF */
}

void lock_close(void)
{
    RELAY = 0;
    LED_GREEN = 1;  /* OFF */
    LED_RED = 0;    /* ON (active low) */
}

/* ========== Password Functions ========== */

unsigned char check_password(void)
{
    unsigned char i;
    for (i = 0; i < PASSWORD_LEN; i++) {
        if (input[i] != password[i]) return 0;
    }
    return 1;
}

void clear_input(void)
{
    unsigned char i;
    for (i = 0; i <= PASSWORD_LEN; i++) input[i] = 0;
    input_pos = 0;
}

void show_input_masked(void)
{
    unsigned char i;
    lcd_goto(1, 0);
    for (i = 0; i < input_pos; i++) lcd_data('*');
    for (i = input_pos; i < PASSWORD_LEN; i++) lcd_data(' ');
}

/* ========== State Handlers ========== */

void show_locked_screen(void)
{
    lcd_clear();
    lcd_goto(0, 0);
    lcd_puts("ENTER PASSWORD:");
    show_input_masked();
}

void show_unlocked_screen(void)
{
    lcd_clear();
    lcd_goto(0, 3);
    lcd_puts("UNLOCKED");
    lcd_goto(1, 1);
    lcd_puts("Press D=Lock");
}

void show_lockout_screen(unsigned char remaining)
{
    lcd_clear();
    lcd_goto(0, 2);
    lcd_puts("LOCKED OUT");
    lcd_goto(1, 3);
    lcd_puts("Wait: ");
    lcd_putnum(remaining);
    lcd_data('s');
}

void show_wrong_password(void)
{
    lcd_clear();
    lcd_goto(0, 0);
    lcd_puts("WRONG PASSWORD");
    lcd_goto(1, 1);
    lcd_putnum(MAX_ATTEMPTS - wrong_attempts);
    lcd_puts(" tries left");
}

void handle_locked_state(void)
{
    char key;

    show_locked_screen();
    clear_input();

    while (current_state == STATE_LOCKED) {
        key = keypad_getkey();

        if (key >= '0' && key <= '9') {
            if (input_pos < PASSWORD_LEN) {
                input[input_pos++] = key;
                beep_short();
                show_input_masked();
            }
        }
        else if (key == 'B') {  /* Backspace */
            if (input_pos > 0) {
                input_pos--;
                input[input_pos] = 0;
                beep_short();
                show_input_masked();
            }
        }
        else if (key == 'C') {  /* Clear */
            clear_input();
            beep_short();
            show_input_masked();
        }
        else if (key == '#') {  /* Enter */
            if (input_pos == PASSWORD_LEN) {
                if (check_password()) {
                    beep_success();
                    wrong_attempts = 0;
                    current_state = STATE_UNLOCKED;
                } else {
                    beep_error();
                    wrong_attempts++;
                    if (wrong_attempts >= MAX_ATTEMPTS) {
                        current_state = STATE_LOCKOUT;
                    } else {
                        show_wrong_password();
                        delay_ms(2000);
                        show_locked_screen();
                        clear_input();
                    }
                }
            }
        }
        else if (key == '*') {  /* Change password (only if unlocked first) */
            /* Ignored in locked state */
        }
    }
}

void handle_unlocked_state(void)
{
    char key;

    lock_open();
    show_unlocked_screen();
    unlock_counter = 0;

    while (current_state == STATE_UNLOCKED) {
        key = keypad_getkey_timeout(1000);

        if (key == 'D') {  /* Manual lock */
            current_state = STATE_LOCKED;
        }
        else if (key == '*') {  /* Change password */
            current_state = STATE_CHANGE;
        }
        else if (key == 0) {
            /* Timeout - check auto-lock */
            unlock_counter++;
            if (unlock_counter >= UNLOCK_TIMEOUT) {
                current_state = STATE_LOCKED;
            }
        }
    }

    lock_close();
}

void handle_lockout_state(void)
{
    unsigned char remaining = LOCKOUT_TIME;

    lock_close();

    while (remaining > 0) {
        show_lockout_screen(remaining);
        delay_ms(1000);
        remaining--;
    }

    wrong_attempts = 0;
    current_state = STATE_LOCKED;
}

void handle_change_password(void)
{
    char key;
    char new_pass[PASSWORD_LEN + 1];
    char confirm[PASSWORD_LEN + 1];
    unsigned char i, pos;

    /* Get new password */
    lcd_clear();
    lcd_goto(0, 0);
    lcd_puts("NEW PASSWORD:");
    pos = 0;

    while (pos < PASSWORD_LEN) {
        key = keypad_getkey();
        if (key >= '0' && key <= '9') {
            new_pass[pos++] = key;
            beep_short();
            lcd_goto(1, 0);
            for (i = 0; i < pos; i++) lcd_data('*');
        }
        else if (key == 'C') {
            current_state = STATE_UNLOCKED;
            return;
        }
    }
    new_pass[PASSWORD_LEN] = 0;

    /* Confirm password */
    lcd_clear();
    lcd_goto(0, 0);
    lcd_puts("CONFIRM:");
    pos = 0;

    while (pos < PASSWORD_LEN) {
        key = keypad_getkey();
        if (key >= '0' && key <= '9') {
            confirm[pos++] = key;
            beep_short();
            lcd_goto(1, 0);
            for (i = 0; i < pos; i++) lcd_data('*');
        }
        else if (key == 'C') {
            current_state = STATE_UNLOCKED;
            return;
        }
    }
    confirm[PASSWORD_LEN] = 0;

    /* Check match */
    for (i = 0; i < PASSWORD_LEN; i++) {
        if (new_pass[i] != confirm[i]) {
            lcd_clear();
            lcd_goto(0, 0);
            lcd_puts("NO MATCH!");
            beep_error();
            delay_ms(2000);
            current_state = STATE_UNLOCKED;
            return;
        }
    }

    /* Save new password */
    for (i = 0; i < PASSWORD_LEN; i++) {
        password[i] = new_pass[i];
    }

    lcd_clear();
    lcd_goto(0, 0);
    lcd_puts("PASSWORD");
    lcd_goto(1, 0);
    lcd_puts("CHANGED!");
    beep_success();
    delay_ms(2000);

    current_state = STATE_UNLOCKED;
}

/* ========== Main ========== */

void main(void)
{
    /* Initialize hardware */
    KEYPAD_PORT = 0xFF;
    RELAY = 0;
    BUZZER = 0;
    LED_GREEN = 1;
    LED_RED = 0;

    lcd_init();

    /* Startup message */
    lcd_goto(0, 2);
    lcd_puts("PASSWORD");
    lcd_goto(1, 4);
    lcd_puts("LOCK");
    delay_ms(2000);

    current_state = STATE_LOCKED;

    while (1) {
        switch (current_state) {
            case STATE_LOCKED:
                handle_locked_state();
                break;
            case STATE_UNLOCKED:
                handle_unlocked_state();
                break;
            case STATE_LOCKOUT:
                handle_lockout_state();
                break;
            case STATE_CHANGE:
                handle_change_password();
                break;
        }
    }
}
