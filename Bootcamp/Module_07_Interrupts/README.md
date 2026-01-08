# Module 07: Interrupts

## Overview
Learn interrupt-driven programming on the 8051 for responsive real-time applications.

## Theory

### Interrupt System
```
                    8051 Interrupt System
    ┌─────────────────────────────────────────────┐
    │                                             │
    │   External     Timer      Serial            │
    │   ┌───┐       ┌───┐      ┌───┐             │
    │   │INT0│      │TF0│      │RI │             │
    │   │INT1│      │TF1│      │TI │             │
    │   └─┬─┘       └─┬─┘      └─┬─┘             │
    │     │           │          │               │
    │     ▼           ▼          ▼               │
    │   ┌─────────────────────────────┐          │
    │   │    Interrupt Controller     │          │
    │   │  ┌────────────────────┐    │          │
    │   │  │   Priority Logic   │    │          │
    │   │  └────────────────────┘    │          │
    │   └──────────────┬──────────────┘          │
    │                  │                         │
    │                  ▼                         │
    │            ┌──────────┐                    │
    │            │   CPU    │                    │
    │            └──────────┘                    │
    └─────────────────────────────────────────────┘
```

### Interrupt Vector Table
```
┌──────────┬─────────┬────────────────────────────┐
│ Address  │  Flag   │   Source                   │
├──────────┼─────────┼────────────────────────────┤
│  0003H   │  IE0    │ External Interrupt 0       │
│  000BH   │  TF0    │ Timer 0 Overflow           │
│  0013H   │  IE1    │ External Interrupt 1       │
│  001BH   │  TF1    │ Timer 1 Overflow           │
│  0023H   │  RI/TI  │ Serial Port (Rx/Tx)        │
└──────────┴─────────┴────────────────────────────┘

SDCC Interrupt Numbers:
  0 = External INT0
  1 = Timer 0
  2 = External INT1
  3 = Timer 1
  4 = Serial Port
```

### IE Register (Interrupt Enable)
```
  IE Register (A8H):
  ┌────┬────┬────┬────┬────┬────┬────┬────┐
  │ EA │ -- │ ET2│ ES │ ET1│ EX1│ ET0│ EX0│
  └────┴────┴────┴────┴────┴────┴────┴────┘
    7    6    5    4    3    2    1    0

  EA  = Global Enable (must be 1 to enable any interrupt)
  ES  = Serial interrupt enable
  ET1 = Timer 1 interrupt enable
  EX1 = External interrupt 1 enable
  ET0 = Timer 0 interrupt enable
  EX0 = External interrupt 0 enable
```

### IP Register (Interrupt Priority)
```
  IP Register (B8H):
  ┌────┬────┬────┬────┬────┬────┬────┬────┐
  │ -- │ -- │ PT2│ PS │ PT1│ PX1│ PT0│ PX0│
  └────┴────┴────┴────┴────┴────┴────┴────┘
    7    6    5    4    3    2    1    0

  1 = High priority
  0 = Low priority (default)

  Default Priority Order (highest to lowest):
  INT0 > Timer0 > INT1 > Timer1 > Serial
```

### External Interrupt Trigger Modes
```
  TCON Register (bits 0-3):
  ┌────┬────┬────┬────┐
  │ IT1│ IE1│ IT0│ IE0│
  └────┴────┴────┴────┘
    3    2    1    0

  IT0/IT1 = 0: Level triggered (low level)
  IT0/IT1 = 1: Edge triggered (falling edge)
```

### SDCC ISR Syntax
```c
// Timer 0 ISR (interrupt 1)
void timer0_isr(void) __interrupt(1)
{
    // ISR code here
    // TF0 is automatically cleared
}

// External INT0 ISR (interrupt 0)
void ext0_isr(void) __interrupt(0)
{
    // ISR code here
}

// Serial ISR (interrupt 4)
void serial_isr(void) __interrupt(4)
{
    if (RI) {
        // Receive handling
        RI = 0;
    }
    if (TI) {
        // Transmit handling
        TI = 0;
    }
}
```

### ISR Guidelines
```
Good Practices:
  ✓ Keep ISRs short and fast
  ✓ Use volatile for shared variables
  ✓ Clear interrupt flags
  ✓ Avoid function calls in ISR
  ✓ Use flags to signal main loop

Bad Practices:
  ✗ Long delays in ISR
  ✗ Printf/blocking calls in ISR
  ✗ Complex calculations
  ✗ Forgetting to clear flags
```

## Hardware Setup

### External Interrupt Circuit
```
                    Vcc
                     │
                    [R] 10K Pull-up
                     │
    P3.2 (INT0) ─────┼──────┐
                     │      │
                   ┌─┴─┐   ─┴─
                   │ S │   GND
                   │ W │
                   └───┘
                  Button

    P1.0-P1.7 ────[LED Array]
```

## Pin Configuration
| Pin  | Function              |
|------|-----------------------|
| P3.2 | INT0 (External Int 0) |
| P3.3 | INT1 (External Int 1) |
| P1   | LED Output            |
| P3.0 | RXD (Serial Rx)       |
| P3.1 | TXD (Serial Tx)       |

## Code Examples

### 01_button_isr.c
External interrupt-driven button press detection.
- Falling edge triggered interrupt
- Debounce using flag mechanism
- LED toggle on each press

### 02_timer_isr.c
Timer overflow interrupt for precise timing.
- 50ms timer interrupt
- 1-second LED toggle (20 interrupts)
- Background processing in main loop

### 03_serial_isr.c
Interrupt-driven serial communication.
- Non-blocking receive/transmit
- Circular buffer for data
- Echo received characters

### 04_stopwatch.c
Complete stopwatch application.
- Timer interrupt for timekeeping
- External interrupt for start/stop
- Seven-segment display output

## Building

```bash
# Build all examples
make

# Build specific example
make build/01_button_isr.ihx

# Clean build files
make clean
```

## Key Concepts

1. **Interrupt Enable**: Set EA=1 (global) + specific interrupt bit
2. **Vector Address**: CPU jumps to fixed address when interrupt occurs
3. **Priority**: Higher priority interrupts can preempt lower ones
4. **Flag Clearing**: Some flags auto-clear, others need manual clear
5. **Volatile**: Always use `volatile` for ISR-shared variables

## Common Mistakes

1. Forgetting to set EA=1 (global enable)
2. Not clearing RI/TI flags in serial ISR
3. Long delays inside ISR blocking other interrupts
4. Race conditions with shared variables
5. Forgetting `volatile` keyword

## Next Steps
- Module 08: 7-Segment & LCD displays
- Module 09: ADC & Sensors
