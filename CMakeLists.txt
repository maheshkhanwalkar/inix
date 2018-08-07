cmake_minimum_required(VERSION 3.12)
project(vminix C ASM)

#-----------------------------------------------------------------------------#
# Build configuration                                                         #
#-----------------------------------------------------------------------------#

set(ARCH x86_64)
set(FMT elf)

#-----------------------------------------------------------------------------#
# Toolchain setup                                                             #
#-----------------------------------------------------------------------------#

enable_language(ASM-ATT)

set(CMAKE_C_COMPILER "${ARCH}-${FMT}-gcc")
set(CMAKE_ASM_COMPILER "${CMAKE_C_COMPILER}")

set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -O2 -ffreestanding\
        -Wno-int-to-pointer-cast -I .")

# Top-level subdirectories
add_subdirectory(arch/${ARCH})
add_subdirectory(kernel)


set(CMAKE_EXE_LINKER_FLAGS "-ffreestanding \
    -T ${CMAKE_SOURCE_DIR}/arch/${ARCH}/linker.ld \
    ${VMINIX_ARCH_LDFLAGS} -nostdlib")

#-----------------------------------------------------------------------------#
# Targets                                                                     #
#-----------------------------------------------------------------------------#

message(STATUS ${SRCS})
add_executable(vminix "${SRCS}")
target_link_libraries(vminix gcc)

#-----------------------------------------------------------------------------#
