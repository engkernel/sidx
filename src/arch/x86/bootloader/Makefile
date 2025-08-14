include $(PROJECT_ROOT)/config.mk

BOOT_DIR := $(BUILD_DIR)/bootloader
SRCS := $(wildcard *.S)
OBJS := $(patsubst %.S,$(BOOT_DIR)/%.o,$(SRCS))

all: $(BOOT_DIR)/boot.bin

$(BOOT_DIR)/%.o: %.S | $(BOOT_DIR)
	$(CC) -c $(TARGET) $< -o $@

$(BOOT_DIR)/boot.elf: $(OBJS)
	$(LD) -m elf_i386 -T boot.ld -o $@ $^

$(BOOT_DIR)/boot.bin: $(BOOT_DIR)/boot.elf
	llvm-objcopy-19 -O binary $< $@

$(BOOT_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(BOOT_DIR)
