
file(GLOB_RECURSE K_SOURCE
        "*.S"
        "*.c"
        "*.h")

# Add sources
set(SRCS "${SRCS};${K_SOURCE}" PARENT_SCOPE)
