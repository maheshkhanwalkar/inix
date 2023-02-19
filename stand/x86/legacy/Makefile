CFLAGS=-ffreestanding -nostdlib -m32
LDFLAGS=--oformat binary -Ttext=0x7c00 --build-id=none -e _legacy_entry -nostdlib

all: stage0

stage0: boot.o
	$(LD) $(LDFLAGS) -o $@ $?

clean:
	rm -rf *.o
	rm stage0
