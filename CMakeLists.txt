cmake_minimum_required(VERSION 3.6)
project(vminix C ASM)

#-----------------------------------------------------------------------------#
# Build configuration                                                         #
#-----------------------------------------------------------------------------#

set(BUILD_ARCH "x86_64" CACHE STRING "Target architecture")

#-----------------------------------------------------------------------------#
# Toolchain setup                                                             #
#-----------------------------------------------------------------------------#

enable_language(ASM-ATT)

# Top-level subdirectories
add_subdirectory(arch/${BUILD_ARCH})
add_subdirectory(kernel)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_ASM_COMPILER "${CMAKE_C_COMPILER}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -Wextra -Werror -ffreestanding\
        -Wno-int-to-pointer-cast")

set(CMAKE_EXE_LINKER_FLAGS "-ffreestanding \
    -fuse-ld=lld -Wl,-T${CMAKE_SOURCE_DIR}/arch/${BUILD_ARCH}/linker.ld \
    ${VMINIX_ARCH_LDFLAGS} -nostdlib")

include_directories(.)

add_executable(vminix "${SRCS}")

#-----------------------------------------------------------------------------#
