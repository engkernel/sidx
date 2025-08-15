include $(PROJECT_ROOT)/config.mk

KERNEL_DIR := $(BUILD_DIR)/kernel

C_SRCS := $(wildcard *.c)
C_OBJS := $(patsubst %.c, $(KERNEL_DIR)/%.o, $(C_SRCS))

all: $(C_OBJS)

$(KERNEL_DIR)/%.o: %.c | $(KERNEL_DIR)
	$(CC) -c $(TARGET) $(CCFLAGS) $< -o $@

$(KERNEL_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(KERNEL_DIR)

	
