# Module 02: I/O Ports - Makefile
CC = sdcc
CFLAGS = -mmcs51
BUILD_DIR = build

SOURCES = $(wildcard src/*.c)
TARGETS = $(SOURCES:src/%.c=$(BUILD_DIR)/%.ihx)

all: $(BUILD_DIR) $(TARGETS)
	@echo "Build complete! Output in $(BUILD_DIR)/"

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.ihx: src/%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
