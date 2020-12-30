set(SOURCES boot/boot.S boot/prep.c)
add_library(x86_64 OBJECT ${SOURCES})
