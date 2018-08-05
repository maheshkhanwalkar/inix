#-----------------------------------------------------------------------------#
# Build configuration (modify here)                                           #

ARCH := x86_64
FMT := elf

#-----------------------------------------------------------------------------#
# Source variable setup                                                       #

SRC_DIRS := arch/$(ARCH) kernel libk

C_SRC := $(shell find $(SRC_DIRS) -type f -name "*.c")
ASM_SRC := $(shell find $(SRC_DIRS) -type f -name "*.S")

OBJS := $(patsubst %.c, %.o, $(C_SRC))
OBJS += $(patsubst %.S, %.o, $(ASM_SRC))

#-----------------------------------------------------------------------------#
# Toolchain setup                                                             #

CC := $(ARCH)-$(FMT)-gcc
CFLAGS := -Wall -Wextra -O2 -ffreestanding -Wno-int-to-pointer-cast -I .
include arch/$(ARCH)/Tconfig

#-----------------------------------------------------------------------------#
# Makefile targets                                                            #

.PHONY=all,clean

all: vminix

vminix: $(OBJS)
	$(CC) $(VMINIX_LD) -ffreestanding -T arch/$(ARCH)/linker.ld $(OBJS) \
		-o vminix -nostdlib -lgcc

clean:
	rm -f $(OBJS) vminix

#-----------------------------------------------------------------------------#
# Extra Makefile targets                                                      #

include arch/$(ARCH)/Econfig

#-----------------------------------------------------------------------------#
