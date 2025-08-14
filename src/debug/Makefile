include $(PROJECT_ROOT)/config.mk

DEBUG_DIR := $(BUILD_DIR)/debug

C_SRCS := $(wildcard *.c)
C_OBJS := $(patsubst %.c, $(DEBUG_DIR)/%.o, $(C_SRCS))

all: $(C_OBJS)

$(DEBUG_DIR)/%.o: %.c | $(DEBUG_DIR)
	$(CC) -c $(TARGET) $(CCFLAGS) $< -o $@

$(DEBUG_DIR):
	mkdir -p $@
