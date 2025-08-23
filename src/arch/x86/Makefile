include $(PROJECT_ROOT)/config.mk

ARCH_DIR := $(BUILD_DIR)/arch

AS_SRCS := $(wildcard *.S)
AS_OBJS := $(patsubst %.S,$(ARCH_DIR)/%.o,$(AS_SRCS))

C_SRCS := $(wildcard *.c)
C_OBJS := $(patsubst %.c,$(ARCH_DIR)/%.o,$(C_SRCS))

all: $(AS_OBJS) $(C_OBJS)

$(ARCH_DIR)/%.o: %.S | $(ARCH_DIR)
	$(CC) -c $(TARGET) $< -o $@

$(ARCH_DIR)/%.o: %.c | $(ARCH_DIR)
	$(CC) -c $(CCFLAGS) $< -o $@

$(ARCH_DIR):
	mkdir -p $@ 

.PHONY: clean
clean:
	rm -rf $(ARCH_DIR)
