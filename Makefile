CC = x86_64-elf-gcc

CFLAGS =-Wall -Wextra -Werror -O2 -ffreestanding -mcmodel=kernel \
		-mno-red-zone \
		-Wno-int-to-pointer-cast \
		-fno-asynchronous-unwind-tables \
		-I .

OBJS = arch/amd64/boot/boot.o arch/amd64/boot/paging.o arch/amd64/boot/gdt.o \
		arch/amd64/boot/parse.o arch/amd64/boot/sse.o kernel/kernel.o libk/string.o

.PHONY=clean,iso

vminix: $(OBJS)
	$(CC) -z max-page-size=0x1000 -ffreestanding -T linker.ld $(OBJS) -o vminix -nostdlib -lgcc

iso: vminix
	cp vminix isodir/boot/vminix
	grub2-mkrescue -o inix.iso isodir

clean:
	rm -f $(OBJS) vminix inix.iso
