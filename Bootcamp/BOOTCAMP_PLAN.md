# 8051 Microcontroller Bootcamp - Learning Plan

> A comprehensive, hands-on bootcamp to master 8051 microcontroller programming using SDCC compiler and Proteus simulation.

## Overview

This bootcamp takes you from zero to hero in 8051 programming through 10 structured modules. Each module includes:
- **Theory** with ASCII diagrams for quick understanding
- **C code examples** that compile with SDCC
- **Proteus simulations** to test without hardware

## Prerequisites

- **SDCC** (Small Device C Compiler): `sudo apt install sdcc`
- **Make**: `sudo apt install make`
- **Proteus** (for simulation)

## Learning Path

```
┌─────────────────────────────────────────────────────────────────┐
│                     8051 BOOTCAMP ROADMAP                       │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  FUNDAMENTALS (Modules 1-4)                                     │
│  ┌──────────┐   ┌──────────┐   ┌──────────┐   ┌──────────┐    │
│  │Module 1  │──▶│Module 2  │──▶│Module 3  │──▶│Module 4  │    │
│  │Intro     │   │I/O Ports │   │Arithmetic│   │Loops &   │    │
│  │          │   │LEDs      │   │Logic     │   │Functions │    │
│  └──────────┘   └──────────┘   └──────────┘   └──────────┘    │
│                                                    │            │
│  ──────────────────────────────────────────────────┘            │
│  │                                                              │
│  ▼ PERIPHERALS (Modules 5-7)                                    │
│  ┌──────────┐   ┌──────────┐   ┌──────────┐                    │
│  │Module 5  │──▶│Module 6  │──▶│Module 7  │                    │
│  │Timers    │   │Serial    │   │Interrupts│                    │
│  │          │   │UART      │   │          │                    │
│  └──────────┘   └──────────┘   └──────────┘                    │
│                                      │                          │
│  ────────────────────────────────────┘                          │
│  │                                                              │
│  ▼ INTERFACING (Modules 8-10)                                   │
│  ┌──────────┐   ┌──────────┐   ┌──────────┐                    │
│  │Module 8  │──▶│Module 9  │──▶│Module 10 │                    │
│  │7-Seg/LCD │   │ADC       │   │Motors &  │                    │
│  │Display   │   │Sensors   │   │Projects  │                    │
│  └──────────┘   └──────────┘   └──────────┘                    │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

---

## Module 1: Introduction to 8051 Architecture

### Learning Objectives
- Understand 8051 vs other microcontrollers
- Learn the pin diagram and internal architecture
- Set up SDCC development environment
- Write and compile your first program

### Theory

#### 8051 Block Diagram
```
┌─────────────────────────────────────────────────────────┐
│                    8051 MICROCONTROLLER                 │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  ┌─────────┐  ┌─────────┐  ┌─────────┐  ┌─────────┐   │
│  │   P0    │  │   P1    │  │   P2    │  │   P3    │   │
│  │ (8-bit) │  │ (8-bit) │  │ (8-bit) │  │ (8-bit) │   │
│  └────┬────┘  └────┬────┘  └────┬────┘  └────┬────┘   │
│       │            │            │            │         │
│  ┌────┴────────────┴────────────┴────────────┴────┐   │
│  │              INTERNAL DATA BUS                  │   │
│  └────┬─────────────┬──────────────┬──────────────┘   │
│       │             │              │                   │
│  ┌────┴────┐   ┌────┴────┐   ┌────┴────┐             │
│  │   CPU   │   │   RAM   │   │  Timers │             │
│  │ (8-bit) │   │ (128B)  │   │ (T0,T1) │             │
│  │   ALU   │   │         │   │         │             │
│  │   ACC   │   │  SFRs   │   │  Serial │             │
│  │    B    │   │ (80-FF) │   │  Port   │             │
│  └─────────┘   └─────────┘   └─────────┘             │
│                                                       │
│  ┌──────────────────────────────────────────────┐    │
│  │              ROM (4KB Program Memory)         │    │
│  │              Address: 0000H - 0FFFH           │    │
│  └──────────────────────────────────────────────┘    │
│                                                       │
│       XTAL1 ──┤├── XTAL2      RST      EA/VPP        │
│              12MHz                                    │
└─────────────────────────────────────────────────────────┘
```

#### 8051 Pin Diagram (40-pin DIP)
```
                    ┌────────────────────┐
            P1.0 ──┤ 1              40 ├── Vcc (+5V)
            P1.1 ──┤ 2              39 ├── P0.0 (AD0)
            P1.2 ──┤ 3              38 ├── P0.1 (AD1)
            P1.3 ──┤ 4              37 ├── P0.2 (AD2)
            P1.4 ──┤ 5              36 ├── P0.3 (AD3)
            P1.5 ──┤ 6              35 ├── P0.4 (AD4)
            P1.6 ──┤ 7              34 ├── P0.5 (AD5)
            P1.7 ──┤ 8              33 ├── P0.6 (AD6)
             RST ──┤ 9              32 ├── P0.7 (AD7)
     (RXD)  P3.0 ──┤ 10    8051    31 ├── EA/VPP
     (TXD)  P3.1 ──┤ 11             30 ├── ALE/PROG
    (INT0)  P3.2 ──┤ 12             29 ├── PSEN
    (INT1)  P3.3 ──┤ 13             28 ├── P2.7 (A15)
      (T0)  P3.4 ──┤ 14             27 ├── P2.6 (A14)
      (T1)  P3.5 ──┤ 15             26 ├── P2.5 (A13)
      (WR)  P3.6 ──┤ 16             25 ├── P2.4 (A12)
      (RD)  P3.7 ──┤ 17             24 ├── P2.3 (A11)
           XTAL2 ──┤ 18             23 ├── P2.2 (A10)
           XTAL1 ──┤ 19             22 ├── P2.1 (A9)
             GND ──┤ 20             21 ├── P2.0 (A8)
                    └────────────────────┘
```

#### Memory Organization
```
CODE MEMORY (ROM)              DATA MEMORY (RAM)
┌───────────────┐              ┌───────────────┐
│    0FFFH      │              │     FFH       │ ◄─ SFR
│               │              ├───────────────┤     (Special Function
│   Program     │              │     80H       │     Registers)
│    Code       │              ├───────────────┤
│               │              │     7FH       │
│               │              │   Scratch     │ ◄─ General Purpose
│               │              │     Pad       │     RAM (30H-7FH)
│               │              │     30H       │
│               │              ├───────────────┤
│               │              │   Bit Addr    │ ◄─ Bit Addressable
│               │              │   20H - 2FH   │     (20H-2FH)
│               │              ├───────────────┤
│               │              │   Bank 3      │ ◄─ Register Banks
│               │              │   Bank 2      │     (00H-1FH)
│               │              │   Bank 1      │     R0-R7 × 4
│    0000H      │              │   Bank 0      │
└───────────────┘              └───────────────┘
```

#### 8051 vs 8052 Comparison
| Feature | 8051 | 8052 |
|---------|------|------|
| ROM | 4 KB | 8 KB |
| RAM | 128 bytes | 256 bytes |
| Timers | 2 (T0, T1) | 3 (T0, T1, T2) |
| Interrupts | 5 | 6 |
| I/O Pins | 32 | 32 |

### Code Example
```c
/* 01_hello_world.c - Your first 8051 program */
#include <8052.h>

void delay(void) {
    unsigned int i;
    for (i = 0; i < 50000; i++);
}

void main(void) {
    while (1) {
        P1 = 0xFF;    // All LEDs ON
        delay();
        P1 = 0x00;    // All LEDs OFF
        delay();
    }
}
```

---

## Module 2: I/O Port Programming

### Learning Objectives
- Understand port structure and operation
- Configure ports as input/output
- Control individual pins using `sbit`
- Interface LEDs and switches

### Theory

#### Port Structure
```
Port Pin Internal Structure (P1, P2, P3):

        Vcc
         │
        [R]  ◄── Internal Pull-up Resistor
         │
         ├────────────────► PIN ◄──────── External Connection
         │
        [Q]  ◄── Output Latch (D Flip-flop)
         │
    Write to ──────┘
      Port
```

#### Port Differences
| Port | Type | Pull-up | Notes |
|------|------|---------|-------|
| P0 | Open-drain | No | Needs external pull-up for input |
| P1 | Quasi-bidirectional | Yes | General purpose I/O |
| P2 | Quasi-bidirectional | Yes | Address bus (high byte) |
| P3 | Quasi-bidirectional | Yes | Alternate functions |

#### P3 Alternate Functions
```
P3.0 ── RXD  (Serial Receive)
P3.1 ── TXD  (Serial Transmit)
P3.2 ── INT0 (External Interrupt 0)
P3.3 ── INT1 (External Interrupt 1)
P3.4 ── T0   (Timer 0 External Input)
P3.5 ── T1   (Timer 1 External Input)
P3.6 ── WR   (External Write)
P3.7 ── RD   (External Read)
```

### Code Examples

#### LED Blinking
```c
/* 01_led_blink.c - Blink LED on P1.0 */
#include <8052.h>

__sbit __at (0x90) LED;  // P1.0

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

void main(void) {
    while (1) {
        LED = 0;         // LED ON (active low)
        delay_ms(500);
        LED = 1;         // LED OFF
        delay_ms(500);
    }
}
```

#### Running LED Pattern
```c
/* 02_led_pattern.c - Knight Rider effect */
#include <8052.h>

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

void main(void) {
    unsigned char pattern = 0x01;
    unsigned char direction = 0;

    while (1) {
        P1 = ~pattern;  // Invert for active-low LEDs
        delay_ms(100);

        if (direction == 0) {
            pattern <<= 1;
            if (pattern == 0x80) direction = 1;
        } else {
            pattern >>= 1;
            if (pattern == 0x01) direction = 0;
        }
    }
}
```

#### Switch Input
```c
/* 03_switch_input.c - Read switch, control LED */
#include <8052.h>

__sbit __at (0x90) LED;     // P1.0
__sbit __at (0x91) SWITCH;  // P1.1

void main(void) {
    SWITCH = 1;  // Configure as input

    while (1) {
        if (SWITCH == 0) {  // Switch pressed (active low)
            LED = 0;        // LED ON
        } else {
            LED = 1;        // LED OFF
        }
    }
}
```

---

## Module 3: Arithmetic & Logic Operations

### Learning Objectives
- Perform arithmetic operations in C
- Use bitwise operators for manipulation
- Implement bit masking techniques
- Create binary counters

### Theory

#### C Operators for 8051
```
Arithmetic:    +   -   *   /   %
Bitwise:       &   |   ^   ~   <<   >>
Comparison:    ==  !=  <   >   <=  >=
Logical:       &&  ||  !
```

#### Bit Masking Examples
```
Set bit 3:     value |= (1 << 3)     // OR with mask
Clear bit 3:   value &= ~(1 << 3)    // AND with inverted mask
Toggle bit 3:  value ^= (1 << 3)     // XOR with mask
Test bit 3:    if (value & (1 << 3)) // AND with mask
```

### Code Examples

#### Binary Counter
```c
/* 04_binary_counter.c - 8-bit counter on LEDs */
#include <8052.h>

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

void main(void) {
    unsigned char count = 0;

    while (1) {
        P1 = ~count;    // Display on LEDs (inverted)
        count++;        // Increment counter
        delay_ms(200);  // 200ms delay
    }
}
```

---

## Module 4: Loops, Functions & Delays

### Learning Objectives
- Calculate accurate delays
- Create reusable delay functions
- Implement software PWM
- Handle button debouncing

### Theory

#### Delay Calculation
```
For 12MHz crystal:
- 1 machine cycle = 12 clock cycles
- 1 machine cycle = 12 / 12MHz = 1 µs

Simple loop:  for (i = 0; i < N; i++);
              Each iteration ≈ 2 machine cycles
              Delay ≈ N × 2 µs
```

### Code Examples

#### Calibrated Delay Library
```c
/* delay.h - Delay functions */
#ifndef DELAY_H
#define DELAY_H

void delay_us(unsigned int us);
void delay_ms(unsigned int ms);

#endif

/* delay.c */
#include "delay.h"

void delay_us(unsigned int us) {
    while (us--) {
        // ~1µs per iteration at 12MHz
    }
}

void delay_ms(unsigned int ms) {
    while (ms--) {
        delay_us(1000);
    }
}
```

---

## Module 5: Timer/Counter Programming

### Learning Objectives
- Understand TMOD and TCON registers
- Program timers in different modes
- Generate accurate time delays
- Use timers as event counters

### Theory

#### TMOD Register
```
┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
│GATE │ C/T │ M1  │ M0  │GATE │ C/T │ M1  │ M0  │
├─────┴─────┴─────┴─────┼─────┴─────┴─────┴─────┤
│      Timer 1          │      Timer 0          │
└───────────────────────┴───────────────────────┘

GATE: 0 = Software start (TRx)
      1 = Hardware start (INTx pin)

C/T:  0 = Timer mode (internal clock)
      1 = Counter mode (external pulses)

M1 M0: Mode selection
  00 = Mode 0: 13-bit timer
  01 = Mode 1: 16-bit timer
  10 = Mode 2: 8-bit auto-reload
  11 = Mode 3: Split timer
```

#### Timer Delay Calculation
```
For 12MHz crystal, 16-bit Mode 1:

Max count = 65536 (0000H to FFFFH)
1 count = 1 µs

For 50ms delay:
  Counts needed = 50000
  Initial value = 65536 - 50000 = 15536 = 3CB0H
  TH0 = 0x3C, TL0 = 0xB0
```

### Code Examples

#### Timer-based Delay
```c
/* 01_timer_delay.c - Precise 1-second delay using Timer 0 */
#include <8052.h>

void timer0_delay_ms(unsigned int ms) {
    unsigned int i;
    TMOD &= 0xF0;       // Clear Timer 0 bits
    TMOD |= 0x01;       // Timer 0, Mode 1 (16-bit)

    for (i = 0; i < ms; i++) {
        TH0 = 0xFC;     // 1ms delay value
        TL0 = 0x66;     // 65536 - 1000 = FC66H
        TR0 = 1;        // Start timer
        while (!TF0);   // Wait for overflow
        TR0 = 0;        // Stop timer
        TF0 = 0;        // Clear flag
    }
}

void main(void) {
    while (1) {
        P1 = ~P1;              // Toggle all LEDs
        timer0_delay_ms(1000); // 1 second
    }
}
```

---

## Module 6: Serial Communication (UART)

### Learning Objectives
- Understand serial communication basics
- Configure baud rate using Timer 1
- Transmit and receive data
- Interface with PC terminal

### Theory

#### Serial Frame Format
```
       ┌─────┬───┬───┬───┬───┬───┬───┬───┬───┬──────┐
Idle ──┤Start│ D0│ D1│ D2│ D3│ D4│ D5│ D6│ D7│ Stop │── Idle
       └─────┴───┴───┴───┴───┴───┴───┴───┴───┴──────┘
         │                                      │
       Bit    Data Bits (LSB first)           Bit
        0                                       1
```

#### Baud Rate Calculation (11.0592 MHz)
```
TH1 = 256 - (11059200 / (384 × BaudRate))

┌────────┬───────┬────────────┐
│ Baud   │  TH1  │  Hex       │
├────────┼───────┼────────────┤
│ 9600   │  253  │  0xFD      │
│ 4800   │  250  │  0xFA      │
│ 2400   │  244  │  0xF4      │
│ 1200   │  232  │  0xE8      │
└────────┴───────┴────────────┘
```

### Code Examples

#### Serial Hello World
```c
/* 01_serial_hello.c - Send string via serial port */
#include <8052.h>

void uart_init(void) {
    TMOD = 0x20;    // Timer 1, Mode 2 (auto-reload)
    TH1 = 0xFD;     // 9600 baud @ 11.0592MHz
    SCON = 0x50;    // 8-bit, 1 stop, REN enabled
    TR1 = 1;        // Start Timer 1
}

void uart_tx(unsigned char c) {
    SBUF = c;
    while (!TI);    // Wait for transmit complete
    TI = 0;         // Clear flag
}

void uart_puts(char *s) {
    while (*s) {
        uart_tx(*s++);
    }
}

void main(void) {
    uart_init();

    while (1) {
        uart_puts("Hello, 8051!\r\n");
        // Add delay here
    }
}
```

---

## Module 7: Interrupts

### Learning Objectives
- Understand interrupt mechanism
- Configure IE and IP registers
- Write interrupt service routines
- Handle multiple interrupt sources

### Theory

#### Interrupt Vector Table
```
┌──────────┬───────────┬──────────────────────┐
│ Address  │  Source   │   Description        │
├──────────┼───────────┼──────────────────────┤
│  0000H   │  Reset    │ Power-on reset       │
│  0003H   │  INT0     │ External Interrupt 0 │
│  000BH   │  TF0      │ Timer 0 Overflow     │
│  0013H   │  INT1     │ External Interrupt 1 │
│  001BH   │  TF1      │ Timer 1 Overflow     │
│  0023H   │  RI/TI    │ Serial Port          │
└──────────┴───────────┴──────────────────────┘
```

#### IE Register (Interrupt Enable)
```
┌────┬────┬────┬────┬────┬────┬────┬────┐
│ EA │ -- │ ET2│ ES │ ET1│ EX1│ ET0│ EX0│
└────┴────┴────┴────┴────┴────┴────┴────┘
  │         │    │    │    │    │    │
  │         │    │    │    │    │    └── External Int 0
  │         │    │    │    │    └─────── Timer 0
  │         │    │    │    └──────────── External Int 1
  │         │    │    └───────────────── Timer 1
  │         │    └────────────────────── Serial Port
  │         └─────────────────────────── Timer 2 (8052)
  └───────────────────────────────────── Global Enable
```

### Code Examples

#### External Interrupt
```c
/* 01_button_isr.c - External interrupt for button */
#include <8052.h>

volatile unsigned char count = 0;

void ext0_isr(void) __interrupt(0) {
    count++;
    P1 = ~count;  // Display on LEDs
}

void main(void) {
    IT0 = 1;      // Edge triggered
    EX0 = 1;      // Enable INT0
    EA = 1;       // Global enable

    while (1) {
        // Main loop - CPU free for other tasks
    }
}
```

---

## Module 8: Seven-Segment & LCD Display

### Learning Objectives
- Interface seven-segment displays
- Implement multiplexing technique
- Initialize and control LCD 16x2
- Display text and numbers

### Theory

#### Seven-Segment Patterns (Common Cathode)
```
     ─a─
    │   │
    f   b
    │   │
     ─g─
    │   │
    e   c
    │   │
     ─d─  ●dp

Segment: dp g  f  e  d  c  b  a
Bit:      7  6  5  4  3  2  1  0

┌───────┬───────────────────────────┬──────┐
│ Digit │  dp  g  f  e  d  c  b  a  │ Hex  │
├───────┼───────────────────────────┼──────┤
│   0   │   0  0  1  1  1  1  1  1  │ 0x3F │
│   1   │   0  0  0  0  0  1  1  0  │ 0x06 │
│   2   │   0  1  0  1  1  0  1  1  │ 0x5B │
│   3   │   0  1  0  0  1  1  1  1  │ 0x4F │
│   4   │   0  1  1  0  0  1  1  0  │ 0x66 │
│   5   │   0  1  1  0  1  1  0  1  │ 0x6D │
│   6   │   0  1  1  1  1  1  0  1  │ 0x7D │
│   7   │   0  0  0  0  0  1  1  1  │ 0x07 │
│   8   │   0  1  1  1  1  1  1  1  │ 0x7F │
│   9   │   0  1  1  0  1  1  1  1  │ 0x6F │
└───────┴───────────────────────────┴──────┘
```

#### LCD 16x2 Commands
```
┌─────────┬─────────────────────────────┐
│ Command │ Description                 │
├─────────┼─────────────────────────────┤
│  0x38   │ 8-bit mode, 2 lines, 5x7    │
│  0x0C   │ Display ON, Cursor OFF      │
│  0x0E   │ Display ON, Cursor ON       │
│  0x0F   │ Display ON, Cursor Blink    │
│  0x01   │ Clear display               │
│  0x02   │ Return home                 │
│  0x06   │ Entry mode: increment       │
│  0x80   │ Set cursor to line 1        │
│  0xC0   │ Set cursor to line 2        │
└─────────┴─────────────────────────────┘
```

### Code Examples

#### Seven-Segment Counter
```c
/* 01_7seg_digit.c - Display 0-9 on 7-segment */
#include <8052.h>

__code unsigned char SEGMENTS[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

void main(void) {
    unsigned char digit = 0;

    while (1) {
        P1 = SEGMENTS[digit];
        digit = (digit + 1) % 10;
        delay_ms(500);
    }
}
```

---

## Module 9: ADC & Sensors

### Learning Objectives
- Interface ADC0804 chip
- Read analog sensors
- Convert readings to meaningful values
- Display sensor data

### Theory

#### ADC0804 Pin Connection
```
           ┌─────────────────┐
      CS ──┤ 1            20 ├── Vcc
      RD ──┤ 2            19 ├── CLK R
      WR ──┤ 3            18 ├── D0 ───► P1.0
   CLK IN ─┤ 4            17 ├── D1 ───► P1.1
    INTR ──┤ 5            16 ├── D2 ───► P1.2
   Vin(+) ─┤ 6            15 ├── D3 ───► P1.3
   Vin(-) ─┤ 7            14 ├── D4 ───► P1.4
    AGND ──┤ 8            13 ├── D5 ───► P1.5
    Vref ──┤ 9            12 ├── D6 ───► P1.6
     GND ──┤ 10           11 ├── D7 ───► P1.7
           └─────────────────┘
```

#### ADC Conversion Formula
```
Digital Value = (Vin / Vref) × 255

For Vref = 5V:
  Vin = (Digital Value / 255) × 5V

For LM35 (10mV/°C):
  Temperature = (Digital Value × 500) / 255
```

### Code Examples

#### Basic ADC Reading
```c
/* 01_adc_read.c - Read ADC0804 and display on LEDs */
#include <8052.h>

#define ADC_DATA P1

__sbit __at (0xB2) RD_PIN;   // P3.2
__sbit __at (0xB3) WR_PIN;   // P3.3
__sbit __at (0xB4) INTR;     // P3.4

unsigned char read_adc(void) {
    unsigned char value;

    WR_PIN = 0;         // Start conversion
    WR_PIN = 1;

    while (INTR == 1);  // Wait for conversion

    RD_PIN = 0;         // Read data
    value = ADC_DATA;
    RD_PIN = 1;

    return value;
}

void main(void) {
    INTR = 1;           // Configure as input
    ADC_DATA = 0xFF;    // Configure as input

    while (1) {
        P2 = read_adc();  // Display on P2
    }
}
```

---

## Module 10: Motors & Final Projects

### Learning Objectives
- Control DC motors with PWM
- Drive stepper motors
- Interface 4x4 keypad
- Build complete projects

### Theory

#### Stepper Motor Sequences
```
Full Step Sequence (Higher Torque):
┌──────┬────┬────┬────┬────┬──────┐
│ Step │ A  │ B  │ C  │ D  │ Hex  │
├──────┼────┼────┼────┼────┼──────┤
│  1   │ 1  │ 0  │ 0  │ 1  │ 0x09 │
│  2   │ 0  │ 0  │ 1  │ 1  │ 0x03 │
│  3   │ 0  │ 1  │ 1  │ 0  │ 0x06 │
│  4   │ 1  │ 1  │ 0  │ 0  │ 0x0C │
└──────┴────┴────┴────┴────┴──────┘

Half Step Sequence (Smoother):
1→0x09, 2→0x01, 3→0x03, 4→0x02,
5→0x06, 6→0x04, 7→0x0C, 8→0x08
```

#### 4x4 Keypad Matrix
```
            Columns (Output)
            C1   C2   C3   C4
            │    │    │    │
    R1 ─────┼────┼────┼────┤  1   2   3   A
            │    │    │    │
Rows R2 ─────┼────┼────┼────┤  4   5   6   B
(Input)     │    │    │    │
    R3 ─────┼────┼────┼────┤  7   8   9   C
            │    │    │    │
    R4 ─────┼────┼────┼────┤  *   0   #   D
```

### Code Examples

#### Stepper Motor Control
```c
/* 02_stepper.c - Stepper motor control */
#include <8052.h>

__code unsigned char STEPS[] = {0x09, 0x03, 0x06, 0x0C};

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

void step_cw(unsigned char steps, unsigned int speed) {
    unsigned char i, s;
    static unsigned char pos = 0;

    for (i = 0; i < steps; i++) {
        P1 = STEPS[pos];
        pos = (pos + 1) % 4;
        delay_ms(speed);
    }
}

void step_ccw(unsigned char steps, unsigned int speed) {
    unsigned char i;
    static unsigned char pos = 0;

    for (i = 0; i < steps; i++) {
        P1 = STEPS[pos];
        pos = (pos == 0) ? 3 : pos - 1;
        delay_ms(speed);
    }
}

void main(void) {
    while (1) {
        step_cw(48, 10);   // One revolution CW
        delay_ms(1000);
        step_ccw(48, 10);  // One revolution CCW
        delay_ms(1000);
    }
}
```

---

## Quick Reference

### SDCC Data Types
| Type | Size | Range |
|------|------|-------|
| `unsigned char` | 8-bit | 0 to 255 |
| `signed char` | 8-bit | -128 to 127 |
| `unsigned int` | 16-bit | 0 to 65535 |
| `signed int` | 16-bit | -32768 to 32767 |
| `__bit` | 1-bit | 0 or 1 |

### SDCC Keywords
```c
__code    // Store in ROM
__data    // Internal RAM (directly addressable)
__idata   // Internal RAM (indirectly addressable)
__xdata   // External RAM
__sbit    // Single bit variable
__sfr     // Special Function Register
__interrupt(n)  // ISR declaration
```

### Build Commands
```bash
# Compile single file
sdcc -mmcs51 program.c -o program.ihx

# Using Makefile
make            # Build all
make clean      # Remove build files
```

---

## Next Steps

After completing this bootcamp, you can:
1. Build real hardware projects
2. Explore advanced features (I2C, SPI)
3. Learn ARM Cortex-M microcontrollers
4. Contribute to the Traffic Flow Control project

Happy Learning! 🎓
