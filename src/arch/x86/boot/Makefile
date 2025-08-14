include $(PROJECT_ROOT)/config.mk

BOOT_DIR := $(BUILD_DIR)/boot

AS_SRCS := $(wildcard *.S)
AS_OBJS := $(patsubst %.S,$(BOOT_DIR)/%.o,$(AS_SRCS))

C_SRCS := $(wildcard *.c)
C_OBJS := $(patsubst %.c,$(BOOT_DIR)/%.o,$(C_SRCS))

all: $(AS_OBJS) $(C_OBJS)

$(BOOT_DIR)/%.o: %.S | $(BOOT_DIR)
	$(CC) -c $(TARGET) $< -o $@

$(BOOT_DIR)/%.o: %.c | $(BOOT_DIR)
	$(CC) -c $(CCFLAGS) $< -o $@

$(BOOT_DIR):
	mkdir -p $@ 
