# Digital Clock

A 24-hour digital clock with 4-digit 7-segment display and time setting buttons.

## Features

- 24-hour format (00:00 - 23:59)
- 4-digit multiplexed 7-segment display
- Hours/Minutes setting with buttons
- Accurate timekeeping using Timer interrupt
- Colon blink every second

## Hardware Requirements

### Components
- 8051/8052 microcontroller
- 4-digit 7-segment display (common cathode)
- 3 push buttons (Mode, Up, Down)
- 11.0592MHz crystal (for accuracy)
- Resistors: 330Ω x 8, 10KΩ x 3

### Circuit Diagram

```
                        Vcc
                         │
                   ┌─────┴─────┐
                   │  330Ω x8  │
                   └─────┬─────┘
                         │
     ┌───────────────────┴───────────────────┐
     │          4-Digit 7-Segment            │
     │   ┌───┐   ┌───┐   ┌───┐   ┌───┐      │
     │   │ H │ : │ H │   │ M │ : │ M │      │
     │   │ H │   │ H │   │ M │   │ M │      │
     │   └───┘   └───┘   └───┘   └───┘      │
     │    D1      D2      D3      D4        │
     └───────────────────┬───────────────────┘
                         │
    Segments ────────────┴──── P1.0-P1.7 (a-g, dp)
    Digit Select ─────────── P2.0-P2.3 (D1-D4)


    Buttons:
                 Vcc
                  │
                [10K] Pull-up
                  │
    P3.2 (MODE)──┼──┬──[SW1]──GND
                  │
    P3.3 (UP) ───┼──┬──[SW2]──GND
                  │
    P3.4 (DOWN)──┼──┬──[SW3]──GND
```

### Pin Assignments

| Pin | Function |
|-----|----------|
| P1.0-P1.6 | Segments a-g |
| P1.7 | Decimal point (colon) |
| P2.0 | Digit 1 (Hours tens) |
| P2.1 | Digit 2 (Hours ones) |
| P2.2 | Digit 3 (Minutes tens) |
| P2.3 | Digit 4 (Minutes ones) |
| P3.2 | Mode button |
| P3.3 | Up button |
| P3.4 | Down button |

## Operating Modes

```
┌─────────────────────────────────────────┐
│              MODE BUTTON                │
│                                         │
│   NORMAL ──► SET_HOURS ──► SET_MINUTES  │
│      ▲                          │       │
│      └──────────────────────────┘       │
│                                         │
│   In SET mode:                          │
│     UP button   = Increment             │
│     DOWN button = Decrement             │
│     MODE button = Next / Exit           │
└─────────────────────────────────────────┘
```

## Display Format

```
Normal Mode:
  ┌───┬───┬───┬───┐
  │ H │ H │ M │ M │    Colon blinks every second
  └───┴───┴───┴───┘

Set Hours Mode:
  ┌───┬───┬───┬───┐
  │ H │ H │ - │ - │    Hours digits blink
  └───┴───┴───┴───┘

Set Minutes Mode:
  ┌───┬───┬───┬───┐
  │ - │ - │ M │ M │    Minutes digits blink
  └───┴───┴───┴───┘
```

## Timing Calculation

Using Timer 0 in Mode 1 with 11.0592MHz crystal:

```
Machine cycle = 12 / 11.0592MHz = 1.085µs
50ms interrupt = 50000 / 1.085 = 46080 cycles
Timer value = 65536 - 46080 = 19456 = 0x4C00

20 interrupts × 50ms = 1 second
```

## Software Architecture

```
┌─────────────────────────────────────────┐
│              Main Loop                  │
│  ┌─────────────────────────────────┐   │
│  │  1. Check button presses        │   │
│  │  2. Handle mode changes         │   │
│  │  3. Update display buffer       │   │
│  │  4. Refresh display (multiplex) │   │
│  └─────────────────────────────────┘   │
└─────────────────────────────────────────┘
            │
            ▼
┌─────────────────────────────────────────┐
│         Timer 0 ISR (50ms)              │
│  ┌─────────────────────────────────┐   │
│  │  1. Reload timer                │   │
│  │  2. Increment tick counter      │   │
│  │  3. If 20 ticks: increment sec  │   │
│  │  4. Handle minute/hour rollover │   │
│  └─────────────────────────────────┘   │
└─────────────────────────────────────────┘
```

## Building

```bash
make
```

Output: `build/digital_clock.ihx`

## Proteus Simulation

1. Load the circuit with 8051, 7-segment displays, and buttons
2. Set crystal frequency to 11.0592MHz
3. Load `build/digital_clock.ihx` into 8051
4. Run simulation

## Files

- `src/main.c` - Main program
- `Makefile` - Build configuration
- `README.md` - This documentation

## Modules Used

This project combines concepts from:
- Module 05: Timers (accurate timekeeping)
- Module 07: Interrupts (Timer ISR)
- Module 08: 7-Segment Display (multiplexing)
- Module 04: Button debouncing
