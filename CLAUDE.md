# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This repository contains 8051 microcontroller programming projects using **SDCC** (Small Device C Compiler) for compilation and **Proteus** for circuit simulation.

## Repository Structure

```
Projects/
  <Project Name>/
    src/                  # C source files
      main.c
    build/                # Compiled output (generated)
    Makefile              # Build configuration
    Proteus Files/        # Circuit simulation files
      *.pdsprj
```

## Build Commands

```bash
cd "Projects/<Project Name>"
make          # Build project -> build/traffic.ihx
make clean    # Remove build artifacts
make info     # Show memory usage
```

## Development Workflow

1. **Write code** in `src/main.c`
2. **Build** with `make` - generates `.ihx` file in `build/`
3. **Simulate** by loading the `.ihx` file in Proteus

## Target Configuration

- **Microcontroller**: Intel 8051/8052
- **Clock**: 12 MHz
- **Compiler**: SDCC with `-mmcs51` flag
- **Header**: `<8052.h>`

## SDCC-Specific Notes

- Use `__code` keyword (not `code`) to store constants in ROM
- Use `__data`, `__idata`, `__xdata` for memory placement
- `bit` type cannot be used as function parameters
- Ports P0-P3 are defined in `<8052.h>`

## 8051 Programming Notes

- Seven-segment patterns use common cathode encoding (`0x3F` = digit 0)
- Software delay loops are calibrated for 12MHz crystal
