# Module 08: 7-Segment & LCD Displays

## Overview
Learn to interface 7-segment displays and 16x2 character LCDs with the 8051.

## Theory

### 7-Segment Display

#### Segment Layout
```
    ┌─────a─────┐
    │           │
    f           b
    │           │
    ├─────g─────┤
    │           │
    e           c
    │           │
    └─────d─────┘  ● dp

  Segment Pattern (Common Cathode):
  Digit:  a  b  c  d  e  f  g  dp   Hex
  ─────────────────────────────────────
    0     1  1  1  1  1  1  0  0   0x3F
    1     0  1  1  0  0  0  0  0   0x06
    2     1  1  0  1  1  0  1  0   0x5B
    3     1  1  1  1  0  0  1  0   0x4F
    4     0  1  1  0  0  1  1  0   0x66
    5     1  0  1  1  0  1  1  0   0x6D
    6     1  0  1  1  1  1  1  0   0x7D
    7     1  1  1  0  0  0  0  0   0x07
    8     1  1  1  1  1  1  1  0   0x7F
    9     1  1  1  1  0  1  1  0   0x6F
```

#### Common Cathode vs Common Anode
```
  Common Cathode:           Common Anode:
    Vcc                       Vcc
     │                         │
    [R]                     to Pin
     │                         │
  Segment ◄── to Pin       Segment
     │                         │
    ─┴─ Common (GND)          [R]
                               │
                              ─┴─ Common (Vcc)

  CC: 1 = ON, 0 = OFF
  CA: 0 = ON, 1 = OFF (inverted)
```

#### Multiplexing 4-Digit Display
```
        Digit 1   Digit 2   Digit 3   Digit 4
          │         │         │         │
    a ────┼─────────┼─────────┼─────────┤
    b ────┼─────────┼─────────┼─────────┤
    c ────┼─────────┼─────────┼─────────┤
    d ────┼─────────┼─────────┼─────────┤  Segment
    e ────┼─────────┼─────────┼─────────┤  Lines
    f ────┼─────────┼─────────┼─────────┤  (shared)
    g ────┼─────────┼─────────┼─────────┤
    dp ───┼─────────┼─────────┼─────────┤
          │         │         │         │
        D1_EN    D2_EN     D3_EN     D4_EN
         (Common select - one at a time)

  Multiplex Timing:
  ┌────┐    ┌────┐    ┌────┐    ┌────┐
  │ D1 │    │ D2 │    │ D3 │    │ D4 │
  └────┘────└────┘────└────┘────└────┘───→ Time
    5ms      5ms      5ms      5ms
         Total: 20ms = 50Hz refresh
```

### 16x2 LCD Display

#### LCD Pinout
```
  ┌─────────────────────────────────────────┐
  │   1  2  3  4  5  6  7  8  9  10 11 12   │
  │  VSS VDD V0 RS RW EN D0 D1 D2 D3 D4 D5  │
  │                                          │
  │  ████████████████████████████████████   │ Line 1
  │  ████████████████████████████████████   │ Line 2
  │                                          │
  │  13 14 15 16                            │
  │  D6 D7 LED+ LED-                        │
  └─────────────────────────────────────────┘

  Pin Functions:
  VSS = GND          V0 = Contrast (pot)
  VDD = +5V          RS = Register Select
  RW = Read/Write    EN = Enable
  D0-D7 = Data Bus   LED+/- = Backlight
```

#### LCD Commands
```
  Command     Hex    Description
  ───────────────────────────────────────
  Clear       0x01   Clear display
  Home        0x02   Return cursor home
  Entry       0x06   Increment, no shift
  Disp ON     0x0C   Display ON, cursor OFF
  Disp+Cur    0x0E   Display ON, cursor ON
  Blink       0x0F   Display ON, cursor blink
  Shift L     0x18   Shift display left
  Shift R     0x1C   Shift display right
  4-bit       0x28   4-bit, 2 line, 5x7
  8-bit       0x38   8-bit, 2 line, 5x7
  Line 1      0x80   Set cursor line 1, col 0
  Line 2      0xC0   Set cursor line 2, col 0
```

#### 4-bit vs 8-bit Mode
```
  8-bit Mode:              4-bit Mode:
  D0-D7 all used          Only D4-D7 used
  1 write per byte        2 writes per byte

  8-bit: Faster, uses 11 pins
  4-bit: Saves 4 pins, slightly slower
```

#### LCD Timing
```
  Command Write (RS=0):
         ┌───────┐
  RS ────┘       └────

  RW ─────────────────  (always 0 for write)

         ┌───────┐
  EN ────┘       └────
              │
         Data valid

  Data Write (RS=1):
         ┌───────────────
  RS ────┘

  RW ─────────────────

         ┌───────┐
  EN ────┘       └────

  EN pulse width: > 450ns
  Data setup time: > 195ns
```

## Hardware Setup

### 7-Segment Circuit
```
              Vcc
               │
        ┌──────┴──────┐
        │   330Ω x8   │
        └──────┬──────┘
               │
        ┌──────┴──────┐
        │  7-Segment  │
        │  (CC type)  │
        └──────┬──────┘
               │
  P1.0-P1.7 ───┘ Segments

  P2.0-P2.3 ──► Digit Select (via transistor)
```

### LCD Circuit (4-bit mode)
```
           LCD 16x2
  ┌──────────────────────┐
  │ 1  2  3  4  5  6     │
  │VSS VDD V0 RS RW EN   │
  │ │  │  │  │  │  │     │
  │GND Vcc POT P2.0 GND P2.1
  │                      │
  │ 11 12 13 14          │
  │ D4 D5 D6 D7          │
  │ │  │  │  │           │
  │P2.4 P2.5 P2.6 P2.7   │
  └──────────────────────┘
```

## Pin Configuration

### 7-Segment
| Pin     | Function          |
|---------|-------------------|
| P1.0-P1.7 | Segment a-g, dp |
| P2.0-P2.3 | Digit select 1-4 |

### LCD (4-bit mode)
| Pin   | Function       |
|-------|----------------|
| P2.0  | RS             |
| P2.1  | EN             |
| P2.4-P2.7 | D4-D7 (data) |

## Code Examples

### 01_7seg_digit.c
Display single digit 0-9 on 7-segment.
- Lookup table for patterns
- Cycle through digits

### 02_7seg_counter.c
4-digit multiplexed counter.
- Time-based multiplexing
- Counts 0000 to 9999
- Digit extraction

### 03_lcd_hello.c
Display "Hello World" on LCD.
- 4-bit initialization sequence
- Command and data functions
- String display

### 04_lcd_counter.c
Running counter on LCD.
- Number to string conversion
- Cursor positioning
- Continuous update

## Building

```bash
# Build all examples
make

# Build specific example
make build/03_lcd_hello.ihx

# Clean build files
make clean
```

## Key Concepts

1. **Multiplexing**: Display multiple digits by rapid switching
2. **Persistence of Vision**: >30Hz appears continuous
3. **LCD Initialization**: Specific sequence required
4. **4-bit Mode**: Send high nibble first, then low nibble
5. **Busy Flag**: LCD needs time between commands

## Common Mistakes

1. Wrong segment pattern (CA vs CC)
2. Multiplex too slow (visible flicker)
3. Forgetting LCD enable pulse
4. Wrong LCD initialization sequence
5. Not waiting for LCD busy flag

## Next Steps
- Module 09: ADC & Sensors
- Module 10: Motors & Projects
