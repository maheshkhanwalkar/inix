set(SOURCES boot/boot.S boot/params.c efi/prep.c mm/pml4.c mm/phys.c mm/vm.c mm/invlpg.S)
add_library(x86_64 OBJECT ${SOURCES})
