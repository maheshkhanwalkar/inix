CC = x86_64-elf-gcc

CFLAGS =-Wall -Wextra -Werror -O2 -ffreestanding -mcmodel=kernel \
		-mno-red-zone -mno-mmx -mno-sse -mno-sse2

OBJS = arch/amd64/boot.o arch/amd64/paging.o arch/amd64/gdt.o

.PHONY=clean,iso

vminix: $(OBJS)
	$(CC) -z max-page-size=0x1000 -O2 -ffreestanding -T linker.ld $(OBJS) -o vminix -nostdlib -lgcc

iso: vminix
	cp vminix isodir/boot/vminix
	grub2-mkrescue -o inix.iso isodir

clean:
	rm -f $(OBJS) vminix inix.iso
