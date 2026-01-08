# Module 01: Introduction - Makefile
# Compiler: SDCC for 8051

CC = sdcc
CFLAGS = -mmcs51
BUILD_DIR = build

# Find all source files
SOURCES = $(wildcard src/*.c)
TARGETS = $(SOURCES:src/%.c=$(BUILD_DIR)/%.ihx)

# Default target
all: $(BUILD_DIR) $(TARGETS)
	@echo "Build complete! Output files in $(BUILD_DIR)/"

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile each source file
$(BUILD_DIR)/%.ihx: src/%.c
	$(CC) $(CFLAGS) $< -o $@
	@echo "Built: $@"

# Clean build files
clean:
	rm -rf $(BUILD_DIR)
	@echo "Cleaned!"

# Show help
help:
	@echo "Usage:"
	@echo "  make       - Build all examples"
	@echo "  make clean - Remove build files"
	@echo ""
	@echo "Output: $(BUILD_DIR)/*.ihx (load into Proteus)"

.PHONY: all clean help
