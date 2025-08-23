include config.mk

PROJECT_ROOT := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
export PROJECT_ROOT

include $(PROJECT_ROOT)/mk/common.mk

BOOTLOADER_BIN := $(BUILD_DIR)/bootloader/boot.bin
KERNEL_ELF := $(BUILD_DIR)/kernel.elf
OS_IMAGE := $(BUILD_DIR)/os.bin
KERNEL_BIN := $(BUILD_DIR)/kernel.bin

all: subdirs link bin

subdirs:
	$(MAKE) -C bootloader
	$(MAKE) -C src 

#OBJS := $(shell find $(BUILD_DIR) -name '*.o' ! -path '$(BUILD_DIR)/bootloader/*' )


link:
	$(LD) -m elf_i386 -T src/linker.ld --Map=$(BUILD_DIR)/kernel.map -nostdlib -o $(KERNEL_ELF) $(shell find $(BUILD_DIR) -name '*.o' ! -path '$(BUILD_DIR)/bootloader/*')
	llvm-objcopy-19 -O binary $(KERNEL_ELF) $(BUILD_DIR)/kernel.bin

bin: $(KERNEL_ELF) $(BOOTLOADER_BIN)
	rm -rf $(OS_IMAGE)
	dd if=$(BOOTLOADER_BIN) >> $(OS_IMAGE)
	dd if=$(KERNEL_BIN) >> $(OS_IMAGE)
	dd if=/dev/zero bs=1048576 count=16 >> $(OS_IMAGE)

.PHONY: clean
clean:
	$(MAKE) -C bootloader clean
	$(MAKE) -C src clean
	rm -f $(BUILD_DIR)/kernel.elf
	rm -f $(BUILD_DIR)/kernel.bin
	rm -f $(BUILD_DIR)/os.bin
