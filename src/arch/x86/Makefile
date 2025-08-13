ifndef BUILD_DIR
	$(error BUILD_DIR is not set)
endif

SUBDIRS := boot bootloader
all: 
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir CC=$(CC) LD=$(LD) BUILD_DIR=$(BUILD_DIR); \
	done
