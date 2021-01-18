set(SOURCES phys.c vm.c alloc/alloc.c alloc/page.c scratch.c)
add_library(mm OBJECT ${SOURCES})
