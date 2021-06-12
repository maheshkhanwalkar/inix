#pragma once

#if defined(CONFIG_ARCH_X86_64) || defined(CONFIG_ARCH_HOSTED)
#define VM_PAGE_SIZE 0x1000
#else
#error VM_PAGE_SIZE not set -- architecture is unknown
#endif
