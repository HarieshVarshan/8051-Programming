# Module 04: Loops, Functions & Delays

## Learning Objectives
- Calculate accurate software delays
- Create reusable delay functions
- Implement software PWM
- Handle button debouncing

## Delay Calculation

### Machine Cycle Timing
```
For 12MHz crystal:
┌─────────────────────────────────────────┐
│ 1 machine cycle = 12 clock cycles       │
│ 1 machine cycle = 12 / 12MHz = 1 µs     │
└─────────────────────────────────────────┘

Simple loop timing:
  for (i = 0; i < N; i++);

  Each iteration ≈ 2 machine cycles
  Total delay ≈ N × 2 µs
```

### Nested Loop for Longer Delays
```c
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)      /* outer loop = ms */
        for (j = 0; j < 120; j++); /* inner loop ≈ 1ms */
}
```

## Software PWM

```
PWM Signal:

     ON time (Duty Cycle)
    ◄───────►
    ┌────────┐        ┌────────┐
    │        │        │        │
────┘        └────────┘        └────────

    ◄──────────────────►
         Period (T)

Duty Cycle = ON time / Period × 100%
```

## Button Debouncing

```
Physical Switch Bouncing:

Pressed        Released
   │              │
   │  ┌─┐ ┌─┐    │  ┌─┐ ┌─┐
   │  │ │ │ │    │  │ │ │ │
───┴──┘ └─┘ └────┴──┘ └─┘ └───

Solution: Wait 10-50ms after detecting change
```

## Code Examples

| File | Description |
|------|-------------|
| `01_delay_functions.c` | Calibrated delay library |
| `02_pwm_led.c` | Software PWM for LED brightness |
| `03_button_debounce.c` | Debounced button input |
| `04_reaction_timer.c` | Simple reaction time game |

## Build & Run

```bash
make
```

## Next Module
[Module 05: Timers →](../Module_05_Timers/)
