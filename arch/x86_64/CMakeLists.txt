set(SOURCES boot/boot.S boot/params.c efi/prep.c
        mm/pml4.c mm/phys.c mm/vm.c mm/invlpg.S
        irq/lidt.S irq/idt.c irq/target.S irq/common.c)

add_library(x86_64 OBJECT ${SOURCES})
