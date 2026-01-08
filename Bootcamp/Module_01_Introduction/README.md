# Module 01: Introduction to 8051 Architecture

## Learning Objectives
- Understand what a microcontroller is
- Learn 8051 internal architecture
- Set up SDCC development environment
- Write and compile your first program

## 8051 Block Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                    8051 MICROCONTROLLER                     │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌─────────┐  ┌─────────┐  ┌─────────┐  ┌─────────┐       │
│  │   P0    │  │   P1    │  │   P2    │  │   P3    │       │
│  │ 8 pins  │  │ 8 pins  │  │ 8 pins  │  │ 8 pins  │       │
│  └────┬────┘  └────┬────┘  └────┬────┘  └────┬────┘       │
│       │            │            │            │             │
│  ┌────┴────────────┴────────────┴────────────┴────┐       │
│  │              INTERNAL DATA BUS                  │       │
│  └────┬─────────────┬──────────────┬──────────────┘       │
│       │             │              │                       │
│  ┌────┴────┐   ┌────┴────┐   ┌────┴────┐                 │
│  │   CPU   │   │   RAM   │   │ Timers  │                 │
│  │         │   │  128B   │   │ T0, T1  │                 │
│  │   ALU   │   │         │   │         │                 │
│  │   ACC   │   │  SFRs   │   │ Serial  │                 │
│  │    B    │   │(80-FFH) │   │  Port   │                 │
│  └─────────┘   └─────────┘   └─────────┘                 │
│                                                           │
│  ┌────────────────────────────────────────────────┐      │
│  │         ROM - 4KB Program Memory               │      │
│  │              (0000H - 0FFFH)                   │      │
│  └────────────────────────────────────────────────┘      │
│                                                           │
│       XTAL1 ──┤├── XTAL2        RST        EA/VPP        │
│              12MHz                                        │
└─────────────────────────────────────────────────────────────┘
```

## Pin Diagram (40-pin DIP)

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
             RST ──┤ 9      8051   32 ├── P0.7 (AD7)
     (RXD)  P3.0 ──┤ 10             31 ├── EA/VPP
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

## Memory Organization

```
  CODE MEMORY (ROM)              DATA MEMORY (RAM)
  ┌───────────────┐              ┌───────────────┐ FFH
  │               │              │     SFRs      │
  │    0FFFH      │              │   (80H-FFH)   │
  │               │              ├───────────────┤ 80H
  │   Program     │              │               │ 7FH
  │     Code      │              │  General RAM  │
  │               │              │   (30H-7FH)   │
  │               │              │               │
  │               │              ├───────────────┤ 30H
  │               │              │Bit Addressable│ 2FH
  │               │              │  (20H-2FH)    │
  │               │              ├───────────────┤ 20H
  │               │              │   Bank 3      │ 1FH
  │               │              │   Bank 2      │ 17H
  │               │              │   Bank 1      │ 0FH
  │    0000H      │              │   Bank 0      │ 00H
  └───────────────┘              └───────────────┘
```

## Key Registers

| Register | Description |
|----------|-------------|
| A (ACC) | Accumulator - main working register |
| B | Used for MUL and DIV operations |
| R0-R7 | General purpose registers (4 banks) |
| DPTR | 16-bit Data Pointer (DPH + DPL) |
| PC | Program Counter (16-bit) |
| SP | Stack Pointer (8-bit, default 07H) |
| PSW | Program Status Word (flags) |

## SDCC Setup

### Data Types
```c
unsigned char   // 8-bit (0 to 255) - MOST USED
signed char     // 8-bit (-128 to 127)
unsigned int    // 16-bit (0 to 65535)
signed int      // 16-bit (-32768 to 32767)
```

### Special Keywords
```c
__code          // Store in ROM (for constants)
__data          // Internal RAM (direct addressing)
__sbit          // Single bit variable
__sfr           // Special Function Register
__interrupt(n)  // ISR declaration
```

### Header File
```c
#include <8052.h>   // For SDCC - defines P0, P1, P2, P3, etc.
```

## Code Example

### Circuit
```
        8051
       ┌─────┐
       │     │
       │  P1 ├──[R]──►LED──┐
       │     │             │
       └─────┘            GND

[R] = 220Ω resistor
LED connected to all 8 pins of P1
```

### Source Code: `01_hello_world.c`
```c
#include <8052.h>

void delay(void) {
    unsigned int i, j;
    for (i = 0; i < 100; i++)
        for (j = 0; j < 1000; j++);
}

void main(void) {
    while (1) {
        P1 = 0x00;    // All LEDs ON (active low)
        delay();
        P1 = 0xFF;    // All LEDs OFF
        delay();
    }
}
```

## Build & Run

```bash
# Build
make

# Output file: build/01_hello_world.ihx

# Load into Proteus simulation or program to hardware
```

## Exercises

1. Modify the delay to make LEDs blink faster/slower
2. Change the pattern to blink alternate LEDs (0x55, 0xAA)
3. Create a counting pattern (0x01, 0x02, 0x04, ...)

## Next Module
[Module 02: I/O Port Programming →](../Module_02_IO_Ports/)
