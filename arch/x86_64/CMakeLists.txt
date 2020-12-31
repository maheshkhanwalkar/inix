set(SOURCES boot/boot.S efi/prep.c mm/pml4.c)
add_library(x86_64 OBJECT ${SOURCES})
