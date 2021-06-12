#include <stack>
#include <array>
#include <stdexcept>

#include <inix/mm/paging.h>
#include <inix/defs.h>

/**
 * Stack of physical memory frames
 */
static std::stack<char*> memory;

/**
 * Number of hosted physical frames
 * TODO: this should be a configurable number, depending on user input
 *  provided to the hosted binary
 */
#define HOSTED_NUM_FRAMES 0x1000 // 16 MiB total memory

extern "C" void arch_phys_init(void)
{
    for(uint64_t i = 0; i < HOSTED_NUM_FRAMES; i++) {
        memory.push(new char[VM_PAGE_SIZE]);
    }
}

extern "C" ptr_t arch_phys_allocate_frame(void)
{
    if(memory.empty()) {
        throw std::runtime_error("out of physical memory");
    }

    char* frame = memory.top();
    memory.pop();

    return reinterpret_cast<ptr_t>(frame);
}

extern "C" void arch_phys_free_frame(ptr_t frame)
{
    char* ptr = reinterpret_cast<char*>(frame);
    memory.push(ptr);
}
