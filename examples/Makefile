# usb_host_KM is CH554-only (the USB Host peripheral is absent on CH552) and
# exceeds the internal RAM of the device under --model-small, so it cannot be
# linked as-is. It is kept in the tree for reference but excluded from the
# default build. To build it manually: make -C usb_host_KM
SUBDIRS = $(filter-out usb_host_KM/, $(shell ls -d */))

.PHONY: $(SUBDIRS) clean

$(SUBDIRS):
	$(MAKE) -C $@

.DEFAULT_GOAL := all
all: $(SUBDIRS)

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

print-%  : ; @echo $* = $($*)


