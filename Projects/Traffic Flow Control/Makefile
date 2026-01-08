# Traffic Flow Control - 8051 Project Makefile
# Compiler: SDCC (Small Device C Compiler)

# Compiler settings
CC = sdcc
CFLAGS = -mmcs51
TARGET = traffic

# Directories
SRC_DIR = src
BUILD_DIR = build

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.rel)

# Main targets
.PHONY: all clean

all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).ihx
	@echo "Build complete: $(BUILD_DIR)/$(TARGET).ihx"

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET).ihx: $(SRCS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR)

# Show memory usage
info: $(BUILD_DIR)/$(TARGET).ihx
	@echo "=== Memory Usage ==="
	@cat $(BUILD_DIR)/$(TARGET).mem 2>/dev/null || echo "Build first with 'make'"

# Help
help:
	@echo "Usage:"
	@echo "  make        - Build the project"
	@echo "  make clean  - Remove build files"
	@echo "  make info   - Show memory usage"
	@echo ""
	@echo "Output: $(BUILD_DIR)/$(TARGET).ihx (load into Proteus)"
