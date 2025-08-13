ARCH ?= x86
CC=clang-19
LD=ld.lld
BUILD_DIR=$(PWD)/build

BOOTLOADER_BIN := $(BUILD_DIR)/bootloader/boot.bin
KERNEL_ELF := $(BUILD_DIR)/kernel.elf
OS_IMAGE := $(BUILD_DIR)/os.bin
KERNEL_BIN := $(BUILD_DIR)/kernel.bin

all: subdirs link bin

subdirs:
	$(MAKE) -C src CC=$(CC) LD=$(LD) BUILD_DIR=$(BUILD_DIR) ARCH=$(ARCH)

OBJS := $(shell find $(BUILD_DIR) -name '*.o' ! -path '$(BUILD_DIR)/bootloader/*' )


link: $(OBJS)
	$(LD) -m elf_i386 -T src/linker.ld -o $(KERNEL_ELF) $(OBJS)
	llvm-objcopy-19 -O binary $(KERNEL_ELF) $(BUILD_DIR)/kernel.bin

bin:  $(KERNEL_ELF) $(BOOTLOADER_BIN)
	# we create a empty image
	dd if=/dev/zero of=$(OS_IMAGE) bs=512 count=100 status=none

	dd if=$(BOOTLOADER_BIN) of=$(OS_IMAGE) bs=512 count=1 conv=notrunc status=none

	dd if=$(KERNEL_BIN) of=$(OS_IMAGE) bs=512 seek=1 conv=notrunc status=none	


.PHONY: clean
clean:
	$(MAKE) -C src clean BUILD_DIR=$(BUILD_DIR)
	rm -f $(BUILD_DIR)/kernel.elf
