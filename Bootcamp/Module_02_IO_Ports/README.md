# Module 02: I/O Port Programming

## Learning Objectives
- Understand port structure and operation
- Configure ports as input/output
- Control individual pins using `sbit`
- Interface LEDs and switches

## Port Structure

```
  Internal Structure of P1/P2/P3 Pin:

         Vcc (+5V)
          │
         [R]  ◄── Internal Pull-up (~50kΩ)
          │
          ├────────────► PIN ◄──── External Circuit
          │
         [Q]  ◄── Output Latch (D Flip-flop)
          │
   Write ─┘

  When you write 1: Pin goes HIGH (pulled up)
  When you write 0: Pin goes LOW (pulled down)
  To read: First write 1, then read pin state
```

## Port Comparison

| Port | Pull-up | Notes |
|------|---------|-------|
| P0 | No (Open-drain) | Needs external pull-up for input |
| P1 | Yes | General purpose I/O |
| P2 | Yes | Also used as address bus (A8-A15) |
| P3 | Yes | Has alternate functions |

## P3 Alternate Functions

```
┌──────┬────────┬─────────────────────────┐
│ Pin  │Function│ Description             │
├──────┼────────┼─────────────────────────┤
│ P3.0 │  RXD   │ Serial Receive          │
│ P3.1 │  TXD   │ Serial Transmit         │
│ P3.2 │  INT0  │ External Interrupt 0    │
│ P3.3 │  INT1  │ External Interrupt 1    │
│ P3.4 │  T0    │ Timer 0 External Input  │
│ P3.5 │  T1    │ Timer 1 External Input  │
│ P3.6 │  WR    │ External Memory Write   │
│ P3.7 │  RD    │ External Memory Read    │
└──────┴────────┴─────────────────────────┘
```

## sbit Declaration

```c
/* Method 1: Using address */
__sbit __at (0x90) LED;     /* P1.0 = address 0x90 */
__sbit __at (0x91) SWITCH;  /* P1.1 = address 0x91 */

/* Port bit addresses:
   P1.0=0x90, P1.1=0x91, ..., P1.7=0x97
   P3.0=0xB0, P3.1=0xB1, ..., P3.7=0xB7
*/
```

## Circuit Diagrams

### LED Connection (Active Low)
```
    8051                    LED
   ┌─────┐                  │
   │     │                 ─┼─
   │ P1.0├───[220Ω]────────►│
   │     │                  │
   └─────┘                 GND

When P1.0 = 0 → LED ON
When P1.0 = 1 → LED OFF
```

### Switch Connection
```
    8051                 Switch
   ┌─────┐                 │
   │     │                 │
   │ P1.1├─────────────────┤
   │     │                 │
   └─────┘                GND

When switch pressed: P1.1 reads 0
When switch open: P1.1 reads 1 (pull-up)
```

## Code Examples

| File | Description |
|------|-------------|
| `01_led_blink.c` | Blink single LED on P1.0 |
| `02_led_pattern.c` | Knight Rider running LED effect |
| `03_switch_input.c` | Read switch, control LED |
| `04_traffic_light.c` | Simple 3-LED traffic light |

## Build & Run

```bash
make            # Build all examples
make clean      # Remove build files
```

## Exercises

1. Create a 4-bit binary counter on P1.0-P1.3
2. Read two switches and perform AND/OR on LEDs
3. Create alternating blink pattern (odd/even LEDs)

## Next Module
[Module 03: Arithmetic & Logic →](../Module_03_Arithmetic_Logic/)
