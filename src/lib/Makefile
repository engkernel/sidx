include $(PROJECT_ROOT)/config.mk

LIB_DIR := $(BUILD_DIR)/lib

C_SRCS := $(wildcard *.c)
C_OBJS := $(patsubst %.c, $(LIB_DIR)/%.o, $(C_SRCS))

all: $(C_OBJS)

$(LIB_DIR)/%.o: %.c | $(LIB_DIR)
	$(CC) -c $(TARGET) $(CCFLAGS) $< -o $@

$(LIB_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(LIB_DIR)
