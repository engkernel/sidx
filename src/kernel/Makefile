CCFLAGS = -target i386-pc-none-eabi -m32
ifndef BUILD_DIR
$(error BUILD_DIR is not set)
endif

KERNEL_DIR := $(BUILD_DIR)/kernel

C_SRCS := $(wildcard *.c)
C_OBJS := $(patsubst %.c, $(KERNEL_DIR)/%.o, $(C_SRCS))

all: $(C_OBJS)

$(KERNEL_DIR)/%.o: %.c | $(KERNEL_DIR)
	$(CC) -c $(CCFLAGS) $< -o $@

$(KERNEL_DIR):
	mkdir -p $@

	
