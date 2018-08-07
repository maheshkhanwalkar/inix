
# x86_64-specific CFLAGS
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcmodel=kernel -mno-red-zone \
    -fno-asynchronous-unwind-tables \
    -mno-sse -mno-sse2 -mno-mmx -mno-3dnow" PARENT_SCOPE)

# x86_64-specific LDFLAGS for vminix
set(VMINIX_ARCH_LDFLAGS "-z max-page-size=0x1000" PARENT_SCOPE)

file(GLOB_RECURSE ARCH_SOURCES
        "*.S"
        "*.c"
        "*.h")

# Add sources
set(SRCS "${SRCS};${ARCH_SOURCES}" PARENT_SCOPE)
