# Components List for 8051 Bootcamp

Complete list of components needed to complete all modules and projects.

---

## Essential Components (Required)

### Microcontroller & Development Board

| Component | Quantity | Notes |
|-----------|----------|-------|
| AT89S52 / AT89C52 | 2 | Main microcontroller (get spare) |
| 40-pin IC socket | 2 | For easy chip replacement |
| 11.0592 MHz Crystal | 2 | For accurate serial timing |
| 12 MHz Crystal | 1 | Alternative for general use |
| 33pF Capacitors | 4 | Crystal load capacitors |
| 10µF Electrolytic Capacitor | 2 | Reset circuit |
| 10KΩ Resistor | 5 | Pull-up, reset circuit |
| Push Button (Reset) | 1 | Reset switch |

### Power Supply

| Component | Quantity | Notes |
|-----------|----------|-------|
| 7805 Voltage Regulator | 1 | 5V regulated output |
| 9V DC Adapter | 1 | Or 9V battery |
| 100µF Capacitor | 2 | Filter capacitors |
| 0.1µF Capacitor | 5 | Decoupling capacitors |
| Power LED + 1KΩ resistor | 1 | Power indicator |

### Programmer

| Component | Quantity | Notes |
|-----------|----------|-------|
| USBasp Programmer | 1 | For flashing hex files |
| 10-pin ISP Cable | 1 | Connects programmer to board |

---

## Module-Specific Components

### Modules 01-04: LEDs & Buttons

| Component | Quantity | Notes |
|-----------|----------|-------|
| Red LED (5mm) | 10 | General purpose |
| Green LED (5mm) | 5 | Status indicators |
| Yellow LED (5mm) | 5 | Traffic light |
| 330Ω Resistor | 20 | LED current limiting |
| Push Button (Tactile) | 5 | Input switches |
| 10KΩ Resistor | 10 | Button pull-ups |

### Module 05: Timers

| Component | Quantity | Notes |
|-----------|----------|-------|
| Buzzer (5V Active) | 1 | For tone generation |
| 100Ω Resistor | 2 | Buzzer current limiting |

### Module 06: Serial Communication

| Component | Quantity | Notes |
|-----------|----------|-------|
| MAX232 IC | 1 | RS232 level converter |
| 16-pin IC Socket | 1 | For MAX232 |
| 1µF Capacitors | 5 | MAX232 charge pump |
| DB9 Female Connector | 1 | Serial port |
| **OR** USB-TTL Module (CP2102/CH340) | 1 | Easier alternative to MAX232 |

### Module 07: Interrupts

| Component | Quantity | Notes |
|-----------|----------|-------|
| Push Buttons | 3 | External interrupts |
| (Uses components from earlier modules) | | |

### Module 08: 7-Segment & LCD

| Component | Quantity | Notes |
|-----------|----------|-------|
| 7-Segment Display (Common Cathode) | 4 | Single digit displays |
| **OR** 4-Digit 7-Segment Module | 1 | Multiplexed display |
| 16x2 LCD Display | 1 | HD44780 compatible |
| 10KΩ Potentiometer | 1 | LCD contrast adjustment |
| 330Ω Resistor | 8 | Segment current limiting |
| BC547 Transistor | 4 | Digit drivers (if needed) |

### Module 09: ADC & Sensors

| Component | Quantity | Notes |
|-----------|----------|-------|
| ADC0804 IC | 1 | 8-bit ADC |
| 20-pin IC Socket | 1 | For ADC0804 |
| LM35 Temperature Sensor | 1 | 10mV/°C output |
| LDR (Light Dependent Resistor) | 2 | Light sensing |
| 10KΩ Potentiometer | 2 | Voltage input, calibration |
| 10KΩ Resistor | 2 | LDR voltage divider |
| 150pF Capacitor | 1 | ADC clock RC |

### Module 10: Motors & Keypad

| Component | Quantity | Notes |
|-----------|----------|-------|
| L293D Motor Driver IC | 1 | DC motor H-bridge |
| 16-pin IC Socket | 1 | For L293D |
| DC Motor (5-12V) | 1 | Small hobby motor |
| 28BYJ-48 Stepper Motor | 1 | With ULN2003 driver |
| ULN2003 Driver Board | 1 | Usually comes with stepper |
| 4x4 Matrix Keypad | 1 | Membrane type |
| 1N4007 Diodes | 4 | Flyback protection |

---

## Project-Specific Components

### Traffic Flow Control

| Component | Quantity | Notes |
|-----------|----------|-------|
| Red LED | 4 | Traffic lights |
| Yellow LED | 4 | Traffic lights |
| Green LED | 4 | Traffic lights |
| 7-Segment Display | 4 | Countdown timers |
| (Most from Module 08) | | |

### Digital Clock

| Component | Quantity | Notes |
|-----------|----------|-------|
| 4-Digit 7-Segment (Common Cathode) | 1 | Time display |
| Push Buttons | 3 | Mode, Up, Down |
| (Uses Module 05, 07, 08 components) | | |

### Password Lock

| Component | Quantity | Notes |
|-----------|----------|-------|
| 4x4 Matrix Keypad | 1 | Password input |
| 16x2 LCD | 1 | Display |
| 5V Relay Module | 1 | Lock control |
| Buzzer | 1 | Audio feedback |
| Green LED | 1 | Unlocked indicator |
| Red LED | 1 | Locked indicator |
| (Uses Module 08, 10 components) | | |

---

## Tools & Equipment

### Essential Tools

| Tool | Notes |
|------|-------|
| Breadboard (830 points) | At least 2 recommended |
| Jumper Wires (M-M, M-F, F-F) | Assorted pack, 100+ pieces |
| Multimeter | For debugging |
| Wire Stripper | For custom wires |
| USB Cable (Type A to Mini/Micro) | For programmer |

### Recommended Tools

| Tool | Notes |
|------|-------|
| Soldering Iron (25-40W) | For permanent projects |
| Solder Wire | Lead-free recommended |
| PCB Holder / Helping Hands | Makes soldering easier |
| Oscilloscope (optional) | For signal debugging |
| Logic Analyzer (optional) | For protocol debugging |

---

## Shopping Summary

### Minimum Kit (Modules 01-06)

```
Qty  Item
───────────────────────────────
1    AT89S52 + 40-pin socket
1    11.0592MHz Crystal
4    33pF Capacitors
2    10µF Capacitors
1    7805 Regulator
1    USBasp Programmer
10   LEDs (assorted colors)
20   330Ω Resistors
10   10KΩ Resistors
5    Push Buttons
1    USB-TTL Module
2    Breadboards
1    Jumper Wire Kit
```

### Complete Kit (All Modules + Projects)

```
Qty  Item
───────────────────────────────
2    AT89S52 + 40-pin socket
2    11.0592MHz Crystal
1    12MHz Crystal
10   33pF Capacitors
5    10µF Capacitors
5    100µF Capacitors
10   0.1µF Capacitors
2    7805 Regulator
1    USBasp Programmer
20   LEDs (Red, Green, Yellow)
30   330Ω Resistors
20   10KΩ Resistors
3    10KΩ Potentiometers
10   Push Buttons
1    USB-TTL Module (CP2102)
1    16x2 LCD Display
2    4-Digit 7-Segment Display
1    ADC0804 + 20-pin socket
1    LM35 Temperature Sensor
2    LDR
1    L293D + 16-pin socket
1    DC Motor (small)
1    28BYJ-48 Stepper + ULN2003
1    4x4 Matrix Keypad
1    5V Relay Module
2    Buzzer (5V active)
5    BC547 Transistors
5    1N4007 Diodes
3    Breadboards
1    Jumper Wire Kit (200pc)
1    Multimeter
```

---

## Where to Buy (India)

- **Amazon.in** - Quick delivery, reliable
- **Robu.in** - Good component variety
- **Electronicscomp.com** - Bulk pricing
- **Quartzcomponents.com** - Wide selection
- **Local electronics market** - Lamington Road (Mumbai), SP Road (Bangalore), Chandni Chowk (Delhi)

## Where to Buy (International)

- **Amazon** - Fast shipping
- **AliExpress** - Cheapest, slow shipping
- **Digi-Key** - Quality components
- **Mouser** - Professional grade
- **Adafruit / SparkFun** - Beginner friendly

---

## Estimated Cost

| Kit | India (₹) | US ($) |
|-----|-----------|--------|
| Minimum Kit | ₹800-1200 | $15-25 |
| Complete Kit | ₹2500-3500 | $40-60 |

*Prices vary by seller and location*

---

## Tips

1. **Buy spares** - ICs and LEDs can fail, get extras
2. **Start small** - Begin with minimum kit, expand later
3. **Quality matters** - Cheap crystals may be inaccurate
4. **Check voltage** - Ensure all components are 5V compatible
5. **Breadboard first** - Test circuits before soldering
