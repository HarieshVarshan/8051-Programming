# Digital Clock - Makefile
CC = sdcc
CFLAGS = -mmcs51
BUILD_DIR = build
TARGET = digital_clock

SRCS = src/main.c

all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).ihx
	@echo "Build complete: $(BUILD_DIR)/$(TARGET).ihx"

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET).ihx: $(SRCS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
