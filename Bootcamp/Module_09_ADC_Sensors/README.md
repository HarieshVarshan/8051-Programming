# Module 09: ADC & Sensors

## Overview
Interface analog-to-digital converters (ADC) and common sensors with the 8051.

## Theory

### Why ADC?
```
  Analog World              Digital World
  ───────────────          ─────────────────
  Temperature              8051 reads 0-255
  Light Level      ─ADC─►  Process data
  Voltage                  Display/Control
  Pressure

  8051 has NO built-in ADC - need external chip
```

### ADC0804 Overview
```
  8-bit ADC with parallel interface
  Resolution: 8 bits (256 levels)
  Input range: 0 to 5V (default)
  Conversion time: ~100µs

  ┌────────────────────────────────────┐
  │           ADC0804 Pinout           │
  ├────────────────────────────────────┤
  │   1  CS      Vcc  20               │
  │   2  RD      CLK  19               │
  │   3  WR      D0   18               │
  │   4  CLK IN  D1   17               │
  │   5  INTR    D2   16               │
  │   6  Vin+    D3   15               │
  │   7  Vin-    D4   14               │
  │   8  AGND    D5   13               │
  │   9  Vref/2  D6   12               │
  │  10  DGND    D7   11               │
  └────────────────────────────────────┘
```

### ADC0804 Timing
```
  Start Conversion:
       ┌───┐
  CS ──┘   └─────────────────
       ┌───┐
  WR ──┘   └─────────────────
            Conversion starts
            (internally)

  Wait for completion:
                    ┌─────────
  INTR ─────────────┘
        ~100µs      Goes LOW when done

  Read Data:
       ┌─────────────┐
  CS ──┘             └───────
           ┌───┐
  RD ──────┘   └─────────────
               Data valid on D0-D7
```

### ADC Calculations
```
  Digital Output = (Vin / Vref) × 256

  Example (Vref = 5V):
  ─────────────────────────────
  Vin     │ Digital │ Binary
  ─────────────────────────────
  0.00V   │    0    │ 00000000
  1.25V   │   64    │ 01000000
  2.50V   │  128    │ 10000000
  3.75V   │  192    │ 11000000
  5.00V   │  255    │ 11111111

  Resolution = 5V / 256 = 19.53mV per step
```

### Common Sensors

#### LM35 Temperature Sensor
```
       ┌─────┐
  Vcc ─┤     ├─ GND
       │LM35 │
       └──┬──┘
          │ Vout
          │
        10mV/°C

  Output: 10mV per degree Celsius
  Range: -55°C to +150°C
  At 25°C: Vout = 250mV

  Temperature = (ADC × 5000) / (256 × 10)
             = (ADC × 500) / 256
             ≈ ADC × 1.96
```

#### LDR (Light Dependent Resistor)
```
         Vcc
          │
         [R] 10K fixed
          │
          ├───► Vout (to ADC)
          │
        [LDR] Variable
          │
         GND

  Light → LDR low → Vout high
  Dark  → LDR high → Vout low
```

#### Potentiometer
```
         Vcc
          │
        ┌─┴─┐
        │   │
        │ P │ Potentiometer
        │ O │
        │ T │
        │   │
        └─┬─┘
          ├───► Wiper (to ADC)
          │
         GND

  Rotation: 0° to 270°
  Output: 0V to Vcc
```

## Hardware Setup

### ADC0804 Circuit
```
                    Vcc
                     │
                     │
            ┌────────┴────────┐
            │    ADC0804      │
   P3.5 ────┤ CS              │
   P3.6 ────┤ RD       D0-D7  ├──── P1.0-P1.7
   P3.7 ────┤ WR              │
   P3.2 ────┤ INTR            │
            │                 │
  Vin+ ─────┤ Vin+    Vref/2  ├──── (open for 0-5V)
            │                 │
            └────────┬────────┘
                     │
                    GND

  CLK: 150pF + 10K gives ~640kHz
```

### Sensor Connections
```
  LM35:                      LDR:
    Vcc ─┬─ Vcc               Vcc
         │                     │
       ┌─┴─┐                  [10K]
       │LM35│                   │
       └─┬─┘                   ─┼─► ADC
         │                     │
         └──► ADC            [LDR]
         │                     │
        GND ─┴─ GND           GND
```

## Pin Configuration
| Pin     | Function              |
|---------|-----------------------|
| P1.0-P1.7 | ADC Data (D0-D7)    |
| P3.5    | CS (Chip Select)      |
| P3.6    | RD (Read)             |
| P3.7    | WR (Write)            |
| P3.2    | INTR (Interrupt)      |

## Code Examples

### 01_adc_read.c
Basic ADC reading with ADC0804.
- Start conversion sequence
- Wait for INTR signal
- Read 8-bit data

### 02_voltmeter.c
Digital voltmeter 0-5V.
- Convert ADC to millivolts
- Display on LEDs or serial
- Fixed point arithmetic

### 03_thermometer.c
Temperature measurement with LM35.
- Read LM35 output via ADC
- Convert to Celsius
- Serial output

### 04_light_meter.c
Light intensity meter with LDR.
- Read LDR voltage
- Calculate light level
- LED bar graph display

## Building

```bash
# Build all examples
make

# Build specific example
make build/03_thermometer.ihx

# Clean build files
make clean
```

## Key Concepts

1. **Start Conversion**: CS=0, WR pulse starts ADC
2. **Wait**: INTR goes low when conversion complete
3. **Read Data**: CS=0, RD=0 to read D0-D7
4. **Resolution**: 8-bit = 256 levels = ~19.5mV/step
5. **Vref**: Sets input voltage range

## Common Mistakes

1. Not waiting for INTR before reading
2. Wrong timing sequence
3. Floating Vref pin (gives half range)
4. Not accounting for sensor offset
5. Integer overflow in calculations

## Formulas Reference
```
  Voltage (mV) = (ADC_value × 5000) / 256
  Temperature = (ADC_value × 500) / 256  (LM35)
  Percentage  = (ADC_value × 100) / 255
```

## Next Steps
- Module 10: Motors & Final Projects
