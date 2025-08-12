CC=clang-19
LD=ld.lld
BUILD_DIR=$(PWD)/build

all:
	$(MAKE) -C src CC=$(CC) LD=$(LD) BUILD_DIR=$(BUILD_DIR)

.PHONY: clean
clean:
	$(MAKE) -C src clean BUILD_DIR=$(BUILD_DIR)
