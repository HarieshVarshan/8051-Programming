# 8051 Microcontroller Bootcamp

> Master 8051 programming from basics to advanced interfacing projects.

```
  ___   ___  ___  _     ____              _
 ( _ ) / _ \| __|| |   | __ )  ___   ___ | |_  ___  __ _  _ __ ___  _ __
 / _ \| | | ||__ \| |   |  _ \ / _ \ / _ \| __|/ __|/ _` || '_ ` _ \| '_ \
| (_) | |_| ||__) | |   | |_) | (_) | (_) | |_| (__| (_| || | | | | | |_) |
 \___/ \___/|____/|_|   |____/ \___/ \___/ \__|\___|\__,_||_| |_| |_| .__/
                                                                    |_|
```

## Prerequisites

```bash
# Install SDCC compiler
sudo apt install sdcc

# Install make
sudo apt install make

# Proteus (for simulation) - Install separately
```

## Learning Path

```
 START
   │
   ▼
┌──────────────────────────────────────────────────────────────┐
│  FUNDAMENTALS                                                │
│  ┌─────────┐   ┌─────────┐   ┌─────────┐   ┌─────────┐     │
│  │Module 1 │──▶│Module 2 │──▶│Module 3 │──▶│Module 4 │     │
│  │  Intro  │   │I/O Ports│   │Arithmet.│   │  Loops  │     │
│  └─────────┘   └─────────┘   └─────────┘   └─────────┘     │
└──────────────────────────────────────────────────────────────┘
                                                    │
                                                    ▼
┌──────────────────────────────────────────────────────────────┐
│  PERIPHERALS                                                 │
│  ┌─────────┐   ┌─────────┐   ┌─────────┐                   │
│  │Module 5 │──▶│Module 6 │──▶│Module 7 │                   │
│  │ Timers  │   │ Serial  │   │Interrupt│                   │
│  └─────────┘   └─────────┘   └─────────┘                   │
└──────────────────────────────────────────────────────────────┘
                                      │
                                      ▼
┌──────────────────────────────────────────────────────────────┐
│  INTERFACING                                                 │
│  ┌─────────┐   ┌─────────┐   ┌─────────┐                   │
│  │Module 8 │──▶│Module 9 │──▶│Module 10│                   │
│  │ Display │   │   ADC   │   │ Motors  │                   │
│  └─────────┘   └─────────┘   └─────────┘                   │
└──────────────────────────────────────────────────────────────┘
                                      │
                                      ▼
                                    DONE!
```

## Modules Overview

| Module | Topic | Description |
|--------|-------|-------------|
| 01 | [Introduction](Module_01_Introduction/) | 8051 architecture, SDCC setup, first program |
| 02 | [I/O Ports](Module_02_IO_Ports/) | LED control, switch input, bit manipulation |
| 03 | [Arithmetic & Logic](Module_03_Arithmetic_Logic/) | Operations, bit masking, counters |
| 04 | [Loops & Functions](Module_04_Loops_Functions/) | Delays, PWM, debouncing |
| 05 | [Timers](Module_05_Timers/) | Timer modes, precise delays, counters |
| 06 | [Serial Communication](Module_06_Serial_Comm/) | UART, baud rate, PC interface |
| 07 | [Interrupts](Module_07_Interrupts/) | ISR, external/timer interrupts |
| 08 | [7-Segment & LCD](Module_08_7Segment_LCD/) | Display interfacing |
| 09 | [ADC & Sensors](Module_09_ADC_Sensors/) | ADC0804, temperature, light sensors |
| 10 | [Motors & Projects](Module_10_Motors_Projects/) | DC/Stepper motors, keypad, final projects |

## Quick Start

```bash
# Navigate to a module
cd Module_01_Introduction

# Build all examples
make

# Clean build files
make clean

# Output files in build/ directory
ls build/
```

## How to Use Each Module

1. **Read** the module's `README.md` for theory
2. **Study** the code examples in `src/`
3. **Build** using `make`
4. **Simulate** in Proteus using files in `simulation/`
5. **Experiment** by modifying the code

## Hardware Requirements (Optional)

For real hardware testing:
- 8051/8052 development board (or AT89S52)
- USB-ASP programmer
- LEDs, resistors, switches
- 7-segment displays, LCD 16x2
- ADC0804, LM35, potentiometer
- Stepper motor, L293D driver

## Resources

- [SDCC Manual](http://sdcc.sourceforge.net/doc/sdccman.pdf)
- [8051 Instruction Set](http://www.keil.com/support/man/docs/is51/)
- [Detailed Plan](BOOTCAMP_PLAN.md)

## Project Structure

```
Bootcamp/
├── README.md               # This file
├── BOOTCAMP_PLAN.md        # Detailed curriculum
├── lib/                    # Shared libraries
│   ├── delay.h
│   ├── lcd.h
│   └── uart.h
├── Module_01_Introduction/
│   ├── README.md
│   ├── Makefile
│   ├── src/
│   │   └── 01_hello_world.c
│   └── simulation/
│       └── hello_world.pdsprj
├── Module_02_IO_Ports/
│   └── ...
└── ...
```

---

**Happy Learning!** Start with [Module 01](Module_01_Introduction/) →
