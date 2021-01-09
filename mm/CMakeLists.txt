set(SOURCES phys.c vm.c alloc.c alloc.h scratch.c scratch.h)
add_library(mm OBJECT ${SOURCES})
