set(SOURCES boot/boot.S boot/params.c efi/prep.c mm/pml4.c mm/phys.c)
add_library(x86_64 OBJECT ${SOURCES})
