# 8051 Programming

A comprehensive learning resource for 8051 microcontroller programming with executable code examples.

## What's Inside

### Bootcamp - Learn 8051 from Scratch

A 10-module course covering everything from basics to advanced topics:

| Module | Topic | Examples |
|--------|-------|----------|
| 01 | Introduction & Setup | 1 |
| 02 | I/O Port Programming | 4 |
| 03 | Arithmetic & Logic | 4 |
| 04 | Loops & Functions | 4 |
| 05 | Timer Programming | 4 |
| 06 | Serial Communication | 4 |
| 07 | Interrupts | 4 |
| 08 | 7-Segment & LCD | 4 |
| 09 | ADC & Sensors | 4 |
| 10 | Motors & Projects | 5 |

**Total: 38 executable code examples**

Each module includes:
- Detailed README with theory and ASCII diagrams
- Working C code examples
- Makefile for easy building

### Projects

| Project | Description |
|---------|-------------|
| Traffic Flow Control | 4-way intersection traffic light with countdown display |
| Digital Clock | 24-hour clock with 7-segment display and time setting |
| Password Lock | Keypad-based security system with LCD and relay |

### Resources

- **COMPONENTS.md** - Shopping list with everything you need to buy
- **TROUBLESHOOTING.md** - Common issues and solutions
- **Shared Libraries** - Reusable code for delays, UART, LCD, ADC

## Getting Started

### Option 1: Docker (Recommended)

No installation required - just Docker.

```bash
# Build and enter the development container
docker compose run --rm dev

# Inside container, build any project
cd Projects/Traffic\ Flow\ Control
make
```

Or build directly without entering the container:

```bash
docker compose run --rm dev make -C "Projects/Traffic Flow Control"
```

### Option 2: Local Installation

```bash
# Install SDCC compiler
sudo apt install sdcc    # Ubuntu/Debian
brew install sdcc        # macOS
```

### Build & Flash

```bash
# Build any module
cd Bootcamp/Module_01_Introduction
make

# Output: build/01_hello_world.ihx
# Flash using USBasp or your preferred programmer
```

## Repository Structure

```
8051-Programming/
├── Bootcamp/
│   ├── Module_01_Introduction/
│   ├── Module_02_IO_Ports/
│   ├── ...
│   ├── Module_10_Motors_Projects/
│   ├── lib/                    # Shared libraries
│   ├── COMPONENTS.md           # Shopping list
│   └── TROUBLESHOOTING.md      # Debug guide
└── Projects/
    ├── Traffic Flow Control/
    ├── Digital_Clock/
    └── Password_Lock/
```

## Hardware

- **Microcontroller:** AT89S52 / AT89C52
- **Crystal:** 11.0592 MHz (for serial) or 12 MHz
- **Programmer:** USBasp

See [COMPONENTS.md](Bootcamp/COMPONENTS.md) for full parts list.

## Tools Used

- **Compiler:** SDCC (Small Device C Compiler)
- **Simulator:** Proteus (optional)
- **Programmer:** USBasp with avrdude

## License

MIT License - Feel free to use for learning and projects.

## Author

**Hariesh Varshan**
Learning and building with 8051!
