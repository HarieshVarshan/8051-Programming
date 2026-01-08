# Module 05: Timer/Counter Programming

## Learning Objectives
- Understand TMOD and TCON registers
- Program timers in different modes
- Generate accurate time delays
- Use timers as event counters

## TMOD Register

```
┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
│GATE │ C/T │ M1  │ M0  │GATE │ C/T │ M1  │ M0  │
├─────┴─────┴─────┴─────┼─────┴─────┴─────┴─────┤
│      Timer 1          │      Timer 0          │
└───────────────────────┴───────────────────────┘

GATE: 0 = Software control (TRx bit)
      1 = Hardware control (INTx pin)

C/T:  0 = Timer mode (internal clock)
      1 = Counter mode (external pulses on Tx pin)

M1 M0: Mode selection
┌────┬────┬────────────────────────────────────┐
│ M1 │ M0 │ Mode                               │
├────┼────┼────────────────────────────────────┤
│  0 │  0 │ Mode 0: 13-bit timer               │
│  0 │  1 │ Mode 1: 16-bit timer               │
│  1 │  0 │ Mode 2: 8-bit auto-reload          │
│  1 │  1 │ Mode 3: Split timer (Timer 0 only) │
└────┴────┴────────────────────────────────────┘
```

## TCON Register

```
┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
│ TF1 │ TR1 │ TF0 │ TR0 │ IE1 │ IT1 │ IE0 │ IT0 │
└─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
  │     │     │     │
  │     │     │     └── Timer 0 Run control
  │     │     └──────── Timer 0 Overflow flag
  │     └────────────── Timer 1 Run control
  └──────────────────── Timer 1 Overflow flag
```

## Delay Calculation (Mode 1, 12MHz)

```
16-bit timer: counts from initial value to FFFFH then overflows

Max count = 65536 (0000H → FFFF → overflow)
1 count = 1 µs (at 12MHz with 12 clock cycles per machine cycle)

For N microseconds delay:
  Initial value = 65536 - N
  TH = (65536 - N) / 256
  TL = (65536 - N) % 256

Example: 50ms (50000 µs) delay
  Initial = 65536 - 50000 = 15536 = 0x3CB0
  TH0 = 0x3C, TL0 = 0xB0
```

## Code Examples

| File | Description |
|------|-------------|
| `01_timer_delay.c` | Precise delays using Timer |
| `02_square_wave.c` | Generate square wave |
| `03_pwm_timer.c` | Hardware PWM with Timer |
| `04_event_counter.c` | Count external events |

## Build & Run

```bash
make
```

## Next Module
[Module 06: Serial Communication →](../Module_06_Serial_Comm/)
