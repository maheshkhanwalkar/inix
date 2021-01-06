#include <arch/x86_64/irq/lidt.h>
#include <arch/x86_64/irq/gdt.h>

#define MAX_IDT_ENTRIES 256

typedef struct idt_entry {
    unsigned short base_low;
    unsigned short segment;
    unsigned short flags;
    unsigned short base_mid;
    unsigned int base_high;
    unsigned int reserved;
} __attribute__((packed)) idt_entry_t;

typedef struct idtr {
    unsigned short limit;
    unsigned long base;
}__attribute__((packed)) idtr_t;

static idt_entry_t idt[MAX_IDT_ENTRIES];
static idtr_t idtr;

void idt_install()
{
    idtr.base = (unsigned long)&idt;
    idtr.limit = MAX_IDT_ENTRIES - 1;

    _lidt(&idtr);
}

void idt_install_handler(unsigned short pos, unsigned long address)
{
    if(pos >= MAX_IDT_ENTRIES)
        return;

    idt[pos].base_low = address & 0xFFFF;
    idt[pos].base_mid = (address >> 16) & 0xFFFF;
    idt[pos].base_high = address >> 32;

    idt[pos].segment = GDT_KERNEL_CODE;
    idt[pos].flags = 0x8E00; /* P + DPL=00 + Interrupt Gate */
}

void irq_arch_setup()
{
    idt_install();
}
