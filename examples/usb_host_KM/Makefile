TARGET = UsbHostHub_KM

# NOTE: USB Host is a CH554-only peripheral (absent on CH552). In addition this
# example exceeds the internal RAM of the device under --model-small, so it does
# not link as-is and is excluded from the top-level examples build.
CHIP = CH554

FREQ_SYS=16000000
EXTRA_FLAGS = -DMAX_PACKET_SIZE=64

XRAM_SIZE = 0x380
XRAM_LOC = 0x080

C_FILES = \
	UsbHostHub_KM.c \
	usbhost.c \
	../../include/debug.c

pre-flash:
	

include ../Makefile.include
