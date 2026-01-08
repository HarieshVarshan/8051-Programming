# Module 06: Serial Communication (UART)

## Learning Objectives
- Understand serial communication basics
- Configure baud rate using Timer 1
- Transmit and receive data
- Interface with PC terminal

## Serial Frame Format

```
       ┌─────┬───┬───┬───┬───┬───┬───┬───┬───┬──────┐
Idle ──┤Start│ D0│ D1│ D2│ D3│ D4│ D5│ D6│ D7│ Stop │── Idle
       └─────┴───┴───┴───┴───┴───┴───┴───┴───┴──────┘
         │                                      │
       LOW                                    HIGH

8 data bits, 1 start bit, 1 stop bit = 10 bits total
```

## SCON Register (Serial Control)

```
┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
│ SM0 │ SM1 │ SM2 │ REN │ TB8 │ RB8 │  TI │  RI │
└─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
  │     │           │                 │     │
  │     │           │                 │     └── Receive Interrupt flag
  │     │           │                 └──────── Transmit Interrupt flag
  │     │           └────────────────────────── Receive Enable
  └─────┴────────────────────────────────────── Mode select

SM0 SM1 Mode:
 0   0   Mode 0: Shift register (synchronous)
 0   1   Mode 1: 8-bit UART, variable baud ◄── Most common
 1   0   Mode 2: 9-bit UART, fixed baud
 1   1   Mode 3: 9-bit UART, variable baud

SCON = 0x50: Mode 1, REN enabled
```

## Baud Rate Calculation

```
Using Timer 1 in Mode 2 (8-bit auto-reload):

          Crystal Frequency
TH1 = 256 - ─────────────────────
             384 × Baud Rate

For 11.0592 MHz crystal:
┌─────────┬──────────┬───────┐
│  Baud   │  TH1     │  Hex  │
├─────────┼──────────┼───────┤
│  9600   │   253    │ 0xFD  │
│  4800   │   250    │ 0xFA  │
│  2400   │   244    │ 0xF4  │
│  1200   │   232    │ 0xE8  │
└─────────┴──────────┴───────┘

Note: 11.0592 MHz gives exact baud rates
      12 MHz has ~0.16% error (usually OK)
```

## Code Examples

| File | Description |
|------|-------------|
| `01_serial_hello.c` | Send "Hello World" |
| `02_serial_echo.c` | Echo received characters |
| `03_serial_menu.c` | Interactive menu system |
| `04_serial_printf.c` | Formatted output |

## Proteus Setup

Use "Virtual Terminal" component:
- Connect RXD (P3.0) → Terminal TX
- Connect TXD (P3.1) → Terminal RX
- Set baud rate to match code

## Build & Run

```bash
make
```

## Next Module
[Module 07: Interrupts →](../Module_07_Interrupts/)
