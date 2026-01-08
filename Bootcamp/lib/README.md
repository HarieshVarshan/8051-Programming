# Shared Libraries

Reusable header-only libraries for 8051 projects.

## Available Libraries

| Library | Description |
|---------|-------------|
| `delay.h` | Software delay functions (us, ms, sec) |
| `uart.h` | UART serial communication (9600 baud default) |
| `lcd.h` | 16x2 LCD in 4-bit mode |
| `adc.h` | ADC0804 interface |

## Usage

Include the library in your source file:

```c
#include "../lib/delay.h"
#include "../lib/uart.h"
```

## Pin Configuration

### Default Pin Assignments

Libraries use default pins which can be overridden:

**UART:**
- Uses standard 8051 serial pins (P3.0=RXD, P3.1=TXD)
- Timer 1 for baud rate generation

**LCD (4-bit mode):**
- P2.0 = RS
- P2.1 = EN
- P2.4-P2.7 = D4-D7

**ADC0804:**
- P3.5 = CS
- P3.6 = RD
- P3.7 = WR
- P3.2 = INTR
- P1 = Data bus

### Custom Pin Configuration

Override defaults before including:

```c
/* Custom LCD pins */
__sbit __at (0xB0) LCD_RS;    /* P3.0 */
__sbit __at (0xB1) LCD_EN;    /* P3.1 */
#define LCD_DATA P2

#include "../lib/lcd.h"
```

## Library Details

### delay.h

```c
void delay_us(unsigned int us);   /* Microsecond delay */
void delay_ms(unsigned int ms);   /* Millisecond delay */
void delay_sec(unsigned int sec); /* Second delay */
```

Calibrated for 12MHz crystal.

### uart.h

```c
void uart_init(void);                    /* Init at 9600 baud */
void uart_init_baud(unsigned char val);  /* Custom baud rate */
void uart_tx(unsigned char c);           /* Send character */
unsigned char uart_rx(void);             /* Receive (blocking) */
unsigned char uart_rx_nb(void);          /* Receive (non-blocking) */
void uart_puts(char *str);               /* Send string */
void uart_putnum(unsigned char num);     /* Send decimal */
void uart_puthex(unsigned char num);     /* Send hex */
void uart_newline(void);                 /* Send CR+LF */

/* Baud rate constants */
#define BAUD_9600  0xFD
#define BAUD_4800  0xFA
#define BAUD_2400  0xF4
```

### lcd.h

```c
void lcd_init(void);                     /* Initialize LCD */
void lcd_clear(void);                    /* Clear display */
void lcd_home(void);                     /* Cursor to home */
void lcd_goto(unsigned char row, col);   /* Set cursor position */
void lcd_data(unsigned char c);          /* Write character */
void lcd_cmd(unsigned char cmd);         /* Send command */
void lcd_puts(char *str);                /* Write string */
void lcd_putnum(unsigned char num);      /* Write decimal (0-255) */
void lcd_putint(unsigned int num);       /* Write int (0-65535) */
void lcd_puthex(unsigned char num);      /* Write hex */

/* LCD commands */
#define LCD_CLEAR      0x01
#define LCD_HOME       0x02
#define LCD_LINE1      0x80
#define LCD_LINE2      0xC0
```

### adc.h

```c
void adc_init(void);                     /* Initialize ADC pins */
void adc_start(void);                    /* Start conversion */
void adc_wait(void);                     /* Wait for completion */
unsigned char adc_read(void);            /* Read result */
unsigned char adc_convert(void);         /* Full conversion */
unsigned int adc_to_mv(unsigned char);   /* Convert to millivolts */
unsigned char adc_to_temp_lm35(unsigned char);  /* LM35 temperature */
unsigned char adc_to_percent(unsigned char);    /* 0-100% */
```

## Example

```c
#include <8052.h>
#include "../lib/delay.h"
#include "../lib/uart.h"
#include "../lib/lcd.h"

void main(void)
{
    uart_init();
    lcd_init();

    lcd_puts("Hello!");
    uart_puts("System Ready\r\n");

    while (1) {
        P1 = 0x00;
        delay_ms(500);
        P1 = 0xFF;
        delay_ms(500);
    }
}
```

## Notes

- Libraries are header-only for simplicity
- All functions are defined in headers (may increase code size if included multiple times)
- For production, consider splitting into .h and .c files
