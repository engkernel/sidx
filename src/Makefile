ifndef BUILD_DIR
	$(error BUILD_DIR is not set)
endif

# default x86
ARCH := x86

ifeq ($(ARCH), riscv)
	$(error ARCH_DIR is not implemented)
endif

SUBDIRS := arch kernel 
all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir ARCH=$(ARCH) CC=$(CC) LD=$(LD) BUILD_DIR=$(BUILD_DIR); \
	done

.PHONY: clean
clean:
	$(MAKE) -C boot clean BUILD_DIR=$(BUILD_DIR)

