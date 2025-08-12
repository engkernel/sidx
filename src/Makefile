ifndef BUILD_DIR
$(error BUILD_DIR is not set)
endif

all:
	$(MAKE) -C boot CC=$(CC) LD=$(LD) BUILD_DIR=$(BUILD_DIR)

.PHONY: clean
clean:
	$(MAKE) -C boot clean BUILD_DIR=$(BUILD_DIR)
	
