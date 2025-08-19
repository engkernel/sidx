include $(PROJECT_ROOT)/config.mk

BOOT_DIR := $(BUILD_DIR)/bootloader
SRCS := $(wildcard *.S)
# we do not compile e820 to fit in 512 byte
OBJS := $(patsubst %.S,$(BOOT_DIR)/%.o,$(SRCS))

BOOT_OBJ := $(BOOT_DIR)/stage1.o

all: $(BOOT_DIR)/boot.bin

$(BOOT_DIR)/%.o: %.S | $(BOOT_DIR)
	$(CC) -c $(TARGET) $< -o $@

# do not link two .o, boot.S should contain all the code
#$(BOOT_DIR)/boot.elf: $(OBJS)
$(BOOT_DIR)/boot.elf: $(BOOT_OBJ)
	$(LD) -m elf_i386 -T boot.ld --Map=$(BOOT_DIR)/boot.map -o $@ $^

$(BOOT_DIR)/boot.bin: $(BOOT_DIR)/boot.elf
	llvm-objcopy-19 -O binary $< $@

$(BOOT_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(BOOT_DIR)
