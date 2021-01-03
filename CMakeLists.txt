cmake_minimum_required(VERSION 3.6)
project(vminix C ASM)

#-----------------------------------------------------------------------------#
# Build configuration                                                         #
#-----------------------------------------------------------------------------#

set(CONFIG_BUILD_ARCH "x86_64" CACHE STRING "Target architecture")
# set(CMAKE_VERBOSE_MAKEFILE 1)

#-----------------------------------------------------------------------------#
# Compiler flags                                                              #
#-----------------------------------------------------------------------------#

set(ADDITIONAL_C_FLAGS "-O0 -g -Wall -Wextra -pedantic -ffreestanding")

if(CONFIG_BUILD_ARCH STREQUAL "x86_64")
    set(ARCH_C_FLAGS "-target x86_64-linux-elf -fPIE -mno-sse -mno-sse2 -mno-mmx -mno-3dnow")
    set(ARCH_LD_FLAGS "-pie")
else()
    message(FATAL_ERROR "Unknown target arch")
endif()

#-----------------------------------------------------------------------------#
# Toolchain setup                                                             #
#-----------------------------------------------------------------------------#

enable_language(ASM-ATT)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_STANDARD 11)
set(CMAKE_ASM_COMPILER "${CMAKE_C_COMPILER}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ADDITIONAL_C_FLAGS} ${ARCH_C_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS "-ffreestanding -Wl,--build-id=none -Wl,--no-dynamic-linker \
    -Wl,-T${CMAKE_SOURCE_DIR}/arch/${CONFIG_BUILD_ARCH}/linker.ld -nostdlib ${ARCH_LD_FLAGS}")

include_directories(.)
include_directories(include)

# Top-level subdirectories
add_subdirectory(mm)
add_subdirectory(kernel)
add_subdirectory(arch/${CONFIG_BUILD_ARCH})

add_executable(vminix /dev/null)
target_link_libraries(vminix kernel mm ${CONFIG_BUILD_ARCH})

#-----------------------------------------------------------------------------#
