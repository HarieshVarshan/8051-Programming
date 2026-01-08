# Troubleshooting Guide

Common issues and solutions when working with 8051 and SDCC.

---

## Build Errors

### Error: `sdcc: command not found`
**Cause:** SDCC not installed or not in PATH.

**Solution:**
```bash
# Ubuntu/Debian
sudo apt install sdcc

# macOS
brew install sdcc

# Verify installation
sdcc --version
```

---

### Error: `undefined reference to '_P1'`
**Cause:** Missing or wrong header file.

**Solution:**
```c
// Use correct header for 8052
#include <8052.h>

// NOT these:
// #include <reg51.h>    // Keil only
// #include <at89x52.h>  // Wrong variant
```

---

### Error: `syntax error near '__sbit'`
**Cause:** Using Keil syntax instead of SDCC syntax.

**Solution:**
```c
// SDCC syntax (correct)
__sbit __at (0x90) LED;

// Keil syntax (wrong for SDCC)
// sbit LED = P1^0;
```

---

### Error: `?ASlink-Error-Could not get 128 bytes`
**Cause:** Code too large for internal RAM.

**Solution:**
```c
// Move constants to code memory
__code unsigned char TABLE[] = {0x3F, 0x06, 0x5B};

// NOT in data memory
// unsigned char TABLE[] = {...};  // Uses precious RAM
```

---

### Warning: `unreferenced function argument`
**Cause:** Function parameter not used.

**Solution:**
```c
// Suppress warning by using the variable
void handler(unsigned char unused) {
    (void)unused;  // Explicitly mark as unused
    // ... rest of code
}
```

---

## Hardware Issues

### LEDs Not Lighting Up

**Check 1:** LED polarity
```
Common Cathode: P1 = 0x00 turns ON
Common Anode:   P1 = 0xFF turns ON (inverted)
```

**Check 2:** Current limiting resistors
```
LED needs 330Ω - 1kΩ resistor in series
Without resistor: LED may burn out or not light
```

**Check 3:** Port configuration
```c
// P0 needs external pull-ups!
// P1, P2, P3 have internal pull-ups
```

---

### LCD Shows Black Boxes Only

**Cause 1:** Contrast not set
```
Adjust V0 pin potentiometer (pin 3)
Too high = blank, Too low = all black
```

**Cause 2:** Initialization sequence wrong
```c
// Must follow exact timing
delay_ms(20);           // Wait for LCD power-up
lcd_nibble(0x30);       // Function set 8-bit
delay_ms(5);            // Wait > 4.1ms
lcd_nibble(0x30);       // Function set 8-bit
delay_us(150);          // Wait > 100us
lcd_nibble(0x30);       // Function set 8-bit
lcd_nibble(0x20);       // Switch to 4-bit mode
```

**Cause 3:** Wrong connections
```
Check: RS, EN, D4-D7 connections
EN pulse must be: HIGH -> LOW (not LOW -> HIGH)
```

---

### LCD Shows Garbage Characters

**Cause:** Timing too fast
```c
// Add delays between operations
void lcd_cmd(unsigned char cmd) {
    LCD_RS = 0;
    lcd_nibble(cmd);
    lcd_nibble(cmd << 4);
    delay_ms(2);  // Add this delay!
}
```

---

### Serial Communication Not Working

**Check 1:** Baud rate mismatch
```c
// For 9600 baud @ 11.0592MHz crystal
TH1 = 0xFD;

// Common TH1 values (11.0592MHz):
// 9600  -> 0xFD
// 4800  -> 0xFA
// 2400  -> 0xF4
// 1200  -> 0xE8
```

**Check 2:** Crystal frequency
```
11.0592MHz = Clean baud rates (recommended)
12MHz = Slight baud rate error (~0.16%)
```

**Check 3:** Terminal settings
```
Baud: 9600
Data bits: 8
Parity: None
Stop bits: 1
Flow control: None
```

**Check 4:** TX/RX swapped
```
8051 TXD (P3.1) -> PC RXD
8051 RXD (P3.0) -> PC TXD
```

---

### Timer Not Counting Correctly

**Check 1:** Timer mode
```c
// Mode 1: 16-bit timer
TMOD = 0x01;  // Timer 0, Mode 1

// Mode 2: 8-bit auto-reload
TMOD = 0x20;  // Timer 1, Mode 2
```

**Check 2:** Timer not started
```c
TR0 = 1;  // Don't forget to start timer!
```

**Check 3:** Timer calculations
```
12MHz crystal, 1 machine cycle = 1µs

Mode 1 (16-bit):
  Max delay = 65536 × 1µs = 65.536ms
  For 50ms: TH0:TL0 = 65536 - 50000 = 15536 = 0x3CB0

Mode 2 (8-bit auto-reload):
  Max delay = 256 × 1µs = 256µs
  Used for baud rate generation
```

---

### Interrupts Not Firing

**Check 1:** Global interrupt enable
```c
EA = 1;  // MUST be set for ANY interrupt to work
```

**Check 2:** Specific interrupt enable
```c
ET0 = 1;  // Timer 0 interrupt
EX0 = 1;  // External INT0
ES = 1;   // Serial interrupt
```

**Check 3:** ISR syntax
```c
// Correct SDCC syntax
void timer0_isr(void) __interrupt(1) {
    // ISR code
}

// Interrupt numbers:
// 0 = External INT0
// 1 = Timer 0
// 2 = External INT1
// 3 = Timer 1
// 4 = Serial
```

**Check 4:** Flag clearing
```c
// Some flags auto-clear, some don't
void serial_isr(void) __interrupt(4) {
    if (RI) {
        RI = 0;  // Must manually clear!
        // handle receive
    }
    if (TI) {
        TI = 0;  // Must manually clear!
        // handle transmit
    }
}
```

---

### ADC Reading Always 0 or 255

**Check 1:** INTR not going low
```c
// Wait for conversion complete
while (ADC_INTR == 1);  // Stuck here = problem

// Check: CS and WR pulse sequence
ADC_CS = 0;
ADC_WR = 0;
delay_us(1);
ADC_WR = 1;  // Rising edge starts conversion
ADC_CS = 1;
```

**Check 2:** Input voltage out of range
```
ADC0804 input: 0V to Vref (typically 5V)
Reading 255 = input >= Vref
Reading 0 = input <= 0V or not connected
```

**Check 3:** Vref/2 pin
```
Pin 9 (Vref/2):
  Open = 0-5V range
  2.5V = 0-5V range
  1.25V = 0-2.5V range
```

---

### Keypad Not Detecting Keys

**Check 1:** Pull-up resistors on columns
```
Columns (inputs) need 10K pull-ups to Vcc
Without pull-ups: floating inputs = random values
```

**Check 2:** Scanning direction
```c
// Rows = output, Columns = input
KEYPAD_PORT = 0xFF;      // All high
KEYPAD_PORT = ~(1 << row); // Drive one row low
// Read columns to find pressed key
```

**Check 3:** Debouncing
```c
key = keypad_scan();
delay_ms(20);            // Debounce delay
if (keypad_scan() == key) {
    // Valid key press
}
```

---

### Stepper Motor Vibrates But Doesn't Turn

**Cause:** Wrong step sequence
```c
// Correct full-step sequence (two-phase)
__code unsigned char STEPS[] = {
    0x03,  // 0011 - A + B
    0x06,  // 0110 - B + A'
    0x0C,  // 1100 - A' + B'
    0x09   // 1001 - B' + A
};

// Wrong (random pattern won't work)
// 0x01, 0x03, 0x07, 0x0F  // WRONG!
```

**Cause:** Step rate too fast
```c
// Stepper needs time to move
// Start slow (10-20ms per step)
// Can speed up once running
delay_ms(10);
```

---

## Simulation Issues (Proteus)

### Simulation Runs But Nothing Happens

**Check 1:** Crystal frequency set
```
Right-click 8051 -> Edit Properties
Set XTAL frequency (11.0592MHz or 12MHz)
```

**Check 2:** Hex file loaded
```
Right-click 8051 -> Edit Properties
Program File: path/to/build/file.ihx
```

**Check 3:** Power supply connected
```
Vcc and GND must be connected
Some components need explicit power
```

---

### Serial Terminal Not Showing Output

**Solution:** Use Virtual Terminal
```
1. Place VIRTUAL TERMINAL component
2. Connect to TXD/RXD pins
3. Set baud rate to match code (9600)
4. Run simulation
```

---

## Code Patterns

### Avoiding Common Mistakes

**Volatile for ISR variables:**
```c
// Always use volatile for variables shared with ISR
volatile unsigned char flag = 0;

void main(void) {
    while (!flag);  // Without volatile, may optimize to infinite loop
}

void isr(void) __interrupt(1) {
    flag = 1;
}
```

**Atomic access:**
```c
// Disable interrupts when accessing multi-byte variables
unsigned int counter;

void read_counter(void) {
    unsigned int temp;
    EA = 0;           // Disable interrupts
    temp = counter;   // Read safely
    EA = 1;           // Re-enable
    return temp;
}
```

**Port read-modify-write:**
```c
// Problem: reading port reads pins, not latch
P1 = 0xF0;
P1 = P1 | 0x01;  // May not work if pins are loaded

// Solution: use shadow register
unsigned char p1_shadow = 0xF0;
P1 = p1_shadow;
p1_shadow |= 0x01;
P1 = p1_shadow;
```

---

## Quick Reference

### Delay Calculation (12MHz)
```
1 machine cycle = 12 clock cycles = 1µs

Software loop: ~2-3 cycles per iteration
for (i = 0; i < 120; i++);  // ~1ms
```

### Memory Limits
```
Internal RAM: 128 bytes (256 for 8052)
SFRs: 128 bytes (80H-FFH)
Code memory: 4KB internal + 64KB external
```

### Common SFR Addresses
```
P0 = 0x80    P1 = 0x90    P2 = 0xA0    P3 = 0xB0
TMOD = 0x89  TCON = 0x88  TH0 = 0x8C   TL0 = 0x8A
SCON = 0x98  SBUF = 0x99  IE = 0xA8    IP = 0xB8
```
