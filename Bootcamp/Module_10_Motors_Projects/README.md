# Module 10: Motors & Final Projects

## Overview
Interface DC motors, stepper motors, and matrix keypads. Combine learned concepts into complete projects.

## Theory

### DC Motor Control

#### H-Bridge Concept
```
         Vcc                    Vcc
          │                      │
      ┌───┴───┐              ┌───┴───┐
      │   S1  │              │   S2  │
      └───┬───┘              └───┬───┘
          │                      │
          ├──────[ MOTOR ]───────┤
          │                      │
      ┌───┴───┐              ┌───┴───┐
      │   S3  │              │   S4  │
      └───┬───┘              └───┬───┘
          │                      │
         GND                    GND

  S1+S4 ON: Forward
  S2+S3 ON: Reverse
  S1+S2 ON: Brake (short)
  All OFF:  Coast
```

#### L293D Motor Driver
```
  ┌───────────────────────────────────┐
  │          L293D Pinout             │
  ├───────────────────────────────────┤
  │  1  EN1        Vcc1  16          │
  │  2  IN1        IN4   15          │
  │  3  OUT1       OUT4  14          │
  │  4  GND        GND   13          │
  │  5  GND        GND   12          │
  │  6  OUT2       OUT3  11          │
  │  7  IN2        IN3   10          │
  │  8  Vcc2       EN2    9          │
  └───────────────────────────────────┘

  EN1 = Enable Motor 1 (PWM here for speed)
  IN1, IN2 = Direction control
  OUT1, OUT2 = Motor connections
```

#### PWM Speed Control
```
  100% Duty:  ████████████████████
               Full speed

   75% Duty:  ███████████████░░░░░
               75% speed

   50% Duty:  ██████████░░░░░░░░░░
               50% speed

   25% Duty:  █████░░░░░░░░░░░░░░░
               25% speed

  PWM Frequency: 500Hz - 20kHz typical
```

### Stepper Motor

#### Step Sequences
```
  Full Step (Wave Drive):
  Step │ Coil A │ Coil B │ Coil A' │ Coil B'
  ─────┼────────┼────────┼─────────┼─────────
    1  │   1    │   0    │    0    │    0
    2  │   0    │   1    │    0    │    0
    3  │   0    │   0    │    1    │    0
    4  │   0    │   0    │    0    │    1

  Full Step (Two-Phase):
  Step │ Coil A │ Coil B │ Coil A' │ Coil B'
  ─────┼────────┼────────┼─────────┼─────────
    1  │   1    │   1    │    0    │    0
    2  │   0    │   1    │    1    │    0
    3  │   0    │   0    │    1    │    1
    4  │   1    │   0    │    0    │    1

  Half Step (8 steps per cycle):
  Higher resolution, smoother motion
```

#### Stepper Patterns
```
  Wave Drive:    0x01 → 0x02 → 0x04 → 0x08
  Two-Phase:     0x03 → 0x06 → 0x0C → 0x09
  Half-Step:     0x01 → 0x03 → 0x02 → 0x06 →
                 0x04 → 0x0C → 0x08 → 0x09
```

### 4x4 Matrix Keypad

#### Keypad Structure
```
        C0    C1    C2    C3
         │     │     │     │
    R0 ──┼──1──┼──2──┼──3──┼──A──
         │     │     │     │
    R1 ──┼──4──┼──5──┼──6──┼──B──
         │     │     │     │
    R2 ──┼──7──┼──8──┼──9──┼──C──
         │     │     │     │
    R3 ──┼──*──┼──0──┼──#──┼──D──

  Rows: Output (drive low one at a time)
  Columns: Input (read which is low)
```

#### Scanning Algorithm
```
  1. Set all rows HIGH
  2. Set R0 = LOW, read columns
     - If C0=LOW → key '1' pressed
     - If C1=LOW → key '2' pressed
     - etc.
  3. Set R0 = HIGH, R1 = LOW
  4. Repeat for all rows
  5. Debounce: Wait 10-20ms, verify
```

## Hardware Setup

### DC Motor Circuit
```
               Vcc (12V)
                 │
          ┌──────┴──────┐
          │   L293D     │
  P1.0 ───┤ EN1    OUT1 ├──┬──[M]──┐
  P1.1 ───┤ IN1    OUT2 ├──┴───────┘
  P1.2 ───┤ IN2         │
          │             │
          └──────┬──────┘
                 │
                GND
```

### Stepper Motor Circuit
```
                 Vcc
                  │
           ┌──────┴──────┐
           │   ULN2003   │
  P1.0 ────┤ IN1   OUT1  ├──── Coil A
  P1.1 ────┤ IN2   OUT2  ├──── Coil B
  P1.2 ────┤ IN3   OUT3  ├──── Coil A'
  P1.3 ────┤ IN4   OUT4  ├──── Coil B'
           │             │
           └──────┬──────┘
                  │
                 GND
```

### Keypad Circuit
```
  8051          4x4 Keypad
  ┌────┐        ┌─────────┐
  │P2.0├────────┤ Row 0   │
  │P2.1├────────┤ Row 1   │
  │P2.2├────────┤ Row 2   │
  │P2.3├────────┤ Row 3   │
  │    │        │         │
  │P2.4├────────┤ Col 0   │
  │P2.5├────────┤ Col 1   │
  │P2.6├────────┤ Col 2   │
  │P2.7├────────┤ Col 3   │
  └────┘        └─────────┘

  Columns need pull-up resistors (10K)
```

## Pin Configuration

### DC Motor
| Pin  | Function    |
|------|-------------|
| P1.0 | Enable (PWM)|
| P1.1 | IN1         |
| P1.2 | IN2         |

### Stepper Motor
| Pin  | Function |
|------|----------|
| P1.0 | Coil A   |
| P1.1 | Coil B   |
| P1.2 | Coil A'  |
| P1.3 | Coil B'  |

### Keypad
| Pin     | Function |
|---------|----------|
| P2.0-P2.3 | Rows   |
| P2.4-P2.7 | Columns|

## Code Examples

### 01_dc_motor.c
DC motor control with PWM speed.
- Forward/Reverse direction
- PWM speed control
- Button control interface

### 02_stepper.c
Stepper motor driver.
- Full step sequence
- Variable speed
- Direction control

### 03_keypad.c
4x4 matrix keypad scanner.
- Row scanning method
- Debounce handling
- Serial output of key

### 04_calculator.c
Simple calculator project.
- Keypad input
- LCD display
- Basic arithmetic

### 05_temp_controller.c
Temperature controller project.
- LM35 sensor input
- Setpoint from keypad
- Relay/LED output
- LCD display

## Building

```bash
# Build all examples
make

# Build specific example
make build/05_temp_controller.ihx

# Clean build files
make clean
```

## Key Concepts

1. **H-Bridge**: Controls motor direction with 4 switches
2. **PWM Duty Cycle**: Percentage ON time controls average voltage
3. **Stepper Sequence**: Energize coils in order for rotation
4. **Matrix Scanning**: Row-by-row scanning for keypad
5. **Debouncing**: Filter out mechanical switch bounce

## Common Mistakes

1. Forgetting flyback diodes on motor coils
2. Wrong stepper sequence (motor vibrates, won't turn)
3. Not debouncing keypad inputs
4. PWM frequency too low (audible noise)
5. Exceeding port current limits (use drivers!)

## Final Projects Integration

These projects combine multiple modules:
```
Calculator:
  Module 08 (LCD) + Module 10 (Keypad)

Temp Controller:
  Module 09 (ADC/LM35) + Module 08 (LCD) +
  Module 10 (Keypad) + Module 02 (I/O)

Traffic Light (existing project):
  Module 08 (7-Segment) + Module 05 (Timers) +
  Module 02 (I/O)
```

## Congratulations!
You've completed the 8051 Bootcamp! You now have the skills to:
- Program all 8051 peripherals
- Interface common sensors and displays
- Control motors and actuators
- Build complete embedded systems

Keep building and experimenting!
