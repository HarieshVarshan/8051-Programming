# Module 03: Arithmetic & Logic Operations

## Learning Objectives
- Perform arithmetic operations in C
- Use bitwise operators for manipulation
- Implement bit masking techniques
- Create binary counters

## C Operators

### Arithmetic Operators
```
+   Addition          a + b
-   Subtraction       a - b
*   Multiplication    a * b
/   Division          a / b
%   Modulus           a % b (remainder)
++  Increment         a++ or ++a
--  Decrement         a-- or --a
```

### Bitwise Operators
```
&   AND         0x0F & 0xF0 = 0x00
|   OR          0x0F | 0xF0 = 0xFF
^   XOR         0x0F ^ 0xFF = 0xF0
~   NOT         ~0x0F = 0xF0
<<  Left shift  0x01 << 3 = 0x08
>>  Right shift 0x80 >> 3 = 0x10
```

## Bit Masking Techniques

```
┌─────────────────────────────────────────────────────────────┐
│ Operation          │ Code                   │ Example       │
├─────────────────────────────────────────────────────────────┤
│ Set bit n          │ value |= (1 << n)      │ Set bit 3     │
│ Clear bit n        │ value &= ~(1 << n)     │ Clear bit 3   │
│ Toggle bit n       │ value ^= (1 << n)      │ Toggle bit 3  │
│ Test bit n         │ if (value & (1 << n))  │ Check bit 3   │
│ Get lower nibble   │ value & 0x0F           │ xxxx____      │
│ Get upper nibble   │ (value >> 4) & 0x0F    │ ____xxxx      │
└─────────────────────────────────────────────────────────────┘
```

## Visual Example: Bit Operations

```
Original:     0101 0011 (0x53)

Set bit 2:    0101 0111 (0x57)
              │  │ │││
              └──┴─┴┴┴── bit 2 set to 1

Clear bit 4:  0100 0011 (0x43)
              │ │
              └─┴── bit 4 cleared

Toggle bit 7: 1101 0011 (0xD3)
              │
              └── bit 7 toggled
```

## Code Examples

| File | Description |
|------|-------------|
| `01_calculator.c` | Basic arithmetic operations |
| `02_bit_masking.c` | Set/clear/toggle bits |
| `03_led_shifter.c` | Shift operations on LEDs |
| `04_binary_counter.c` | 8-bit counter display |

## Build & Run

```bash
make
```

## Next Module
[Module 04: Loops & Functions →](../Module_04_Loops_Functions/)
