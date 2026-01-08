# Password Lock System

A digital lock system with 4x4 keypad input, LCD display, and relay-controlled lock.

## Features

- 4-digit password protection
- LCD display with masked input (****)
- 3 wrong attempts lockout (30 seconds)
- Password change functionality
- Relay/solenoid lock control
- Buzzer feedback

## Hardware Requirements

### Components
- 8051/8052 microcontroller
- 16x2 LCD display
- 4x4 matrix keypad
- Relay module (5V)
- Buzzer
- 12MHz crystal

### Circuit Diagram

```
                    ┌──────────────────────┐
                    │     16x2 LCD         │
                    │  ┌────────────────┐  │
                    │  │ ENTER PASSWORD │  │
                    │  │ ****           │  │
                    │  └────────────────┘  │
                    └──────────┬───────────┘
                               │
    P2.0 = RS ─────────────────┤
    P2.1 = EN ─────────────────┤
    P2.4-P2.7 = D4-D7 ─────────┘

                    ┌──────────────────────┐
                    │    4x4 Keypad        │
                    │  ┌──┬──┬──┬──┐       │
                    │  │1 │2 │3 │A │       │
                    │  ├──┼──┼──┼──┤       │
                    │  │4 │5 │6 │B │       │
                    │  ├──┼──┼──┼──┤       │
                    │  │7 │8 │9 │C │       │
                    │  ├──┼──┼──┼──┤       │
                    │  │* │0 │# │D │       │
                    │  └──┴──┴──┴──┘       │
                    └──────────┬───────────┘
                               │
    P1.0-P1.3 = Rows ──────────┤
    P1.4-P1.7 = Cols ──────────┘

    Output Controls:
                         Vcc
                          │
    P3.0 (RELAY) ────[R]──┴──[Relay Coil]──► Lock
                     1K

    P3.1 (BUZZER) ───[R]──────[Buzzer]────► GND
                     100Ω

    P3.2 (LED_G) ────[R]──────[Green LED]─► GND
                     330Ω

    P3.3 (LED_R) ────[R]──────[Red LED]───► GND
                     330Ω
```

### Pin Assignments

| Pin | Function |
|-----|----------|
| P1.0-P1.3 | Keypad rows |
| P1.4-P1.7 | Keypad columns |
| P2.0 | LCD RS |
| P2.1 | LCD EN |
| P2.4-P2.7 | LCD D4-D7 |
| P3.0 | Relay (Lock) |
| P3.1 | Buzzer |
| P3.2 | Green LED (Unlocked) |
| P3.3 | Red LED (Locked) |

## Keypad Functions

```
┌─────┬─────┬─────┬─────┐
│  1  │  2  │  3  │  A  │  A = Not used
├─────┼─────┼─────┼─────┤
│  4  │  5  │  6  │  B  │  B = Backspace
├─────┼─────┼─────┼─────┤
│  7  │  8  │  9  │  C  │  C = Clear
├─────┼─────┼─────┼─────┤
│  *  │  0  │  #  │  D  │  * = Change password
└─────┴─────┴─────┴─────┘  # = Enter/Confirm
                           D = Lock (manual)
```

## Operating States

```
┌─────────────────────────────────────────────────┐
│                  STATE MACHINE                  │
│                                                 │
│   ┌──────────┐                                  │
│   │  LOCKED  │◄─────────────────────────┐      │
│   └────┬─────┘                          │      │
│        │ Enter password                 │      │
│        ▼                                │      │
│   ┌──────────┐    Wrong (3x)    ┌───────┴────┐ │
│   │ CHECKING ├─────────────────►│  LOCKOUT   │ │
│   └────┬─────┘                  │  (30 sec)  │ │
│        │ Correct                └────────────┘ │
│        ▼                                │      │
│   ┌──────────┐                          │      │
│   │ UNLOCKED │──────────────────────────┘      │
│   └────┬─────┘    Press D or timeout          │
│        │ Press *                               │
│        ▼                                       │
│   ┌──────────┐                                 │
│   │ CHANGE   │─────────────────────────────────┘
│   │ PASSWORD │    Enter new password twice
│   └──────────┘
└─────────────────────────────────────────────────┘
```

## LCD Messages

```
Locked State:
  ┌────────────────┐
  │ENTER PASSWORD: │
  │****            │
  └────────────────┘

Unlocked State:
  ┌────────────────┐
  │   UNLOCKED     │
  │  Press D=Lock  │
  └────────────────┘

Wrong Password:
  ┌────────────────┐
  │ WRONG PASSWORD │
  │  2 tries left  │
  └────────────────┘

Lockout:
  ┌────────────────┐
  │   LOCKED OUT   │
  │   Wait: 25s    │
  └────────────────┘

Change Password:
  ┌────────────────┐
  │NEW PASSWORD:   │
  │****            │
  └────────────────┘
```

## Default Password

Default password: `1234`

## Building

```bash
make
```

Output: `build/password_lock.ihx`

## Security Features

1. **Masked Input**: Password shown as asterisks
2. **Lockout**: 30-second lockout after 3 wrong attempts
3. **Auto-Lock**: Returns to locked state after timeout
4. **Buzzer Feedback**: Different tones for success/failure

## Files

- `src/main.c` - Main program
- `Makefile` - Build configuration
- `README.md` - This documentation

## Modules Used

This project combines concepts from:
- Module 08: LCD Display
- Module 10: Matrix Keypad
- Module 02: I/O Ports (Relay, LED, Buzzer)
- Module 04: Delay functions
