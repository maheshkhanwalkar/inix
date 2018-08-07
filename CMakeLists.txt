cmake_minimum_required(VERSION 3.6)
project(vminix C ASM)

#-----------------------------------------------------------------------------#
# Build configuration                                                         #
#-----------------------------------------------------------------------------#

set(BUILD_ARCH "x86_64" CACHE FILEPATH "Target architecture")

#-----------------------------------------------------------------------------#
# Toolchain setup                                                             #
#-----------------------------------------------------------------------------#

enable_language(ASM-ATT)

# Top-level subdirectories
add_subdirectory(arch/${BUILD_ARCH})
add_subdirectory(kernel)

set(CMAKE_C_COMPILER "${BUILD_ARCH}-${BUILD_FMT}-gcc")
set(CMAKE_ASM_COMPILER "${CMAKE_C_COMPILER}")

set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -O2 -ffreestanding\
        -Wno-int-to-pointer-cast -I .")

set(CMAKE_EXE_LINKER_FLAGS "-ffreestanding \
    -T ${CMAKE_SOURCE_DIR}/arch/${BUILD_ARCH}/linker.ld \
    ${VMINIX_ARCH_LDFLAGS} -nostdlib")

add_executable(vminix "${SRCS}")
target_link_libraries(vminix gcc)

#-----------------------------------------------------------------------------#
