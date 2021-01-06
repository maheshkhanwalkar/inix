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

// ISR references (see arch/x86_64/irq/target.S)
extern void __isr0();
extern void __isr1();
extern void __isr2();
extern void __isr3();
extern void __isr4();
extern void __isr5();
extern void __isr6();
extern void __isr7();
extern void __isr8();
extern void __isr9();
extern void __isr10();
extern void __isr11();
extern void __isr12();
extern void __isr13();
extern void __isr14();
extern void __isr15();
extern void __isr16();
extern void __isr17();
extern void __isr18();
extern void __isr19();
extern void __isr20();
extern void __isr21();
extern void __isrReserved();

void irq_arch_setup()
{
    // ISR 0-21 stub entries
    idt_install_handler(0, (unsigned long)&__isr0);
    idt_install_handler(1, (unsigned long)&__isr1);
    idt_install_handler(2, (unsigned long)&__isr2);
    idt_install_handler(3, (unsigned long)&__isr3);
    idt_install_handler(4, (unsigned long)&__isr4);
    idt_install_handler(5, (unsigned long)&__isr5);
    idt_install_handler(6, (unsigned long)&__isr6);
    idt_install_handler(7, (unsigned long)&__isr7);
    idt_install_handler(8, (unsigned long)&__isr8);
    idt_install_handler(9, (unsigned long)&__isr9);
    idt_install_handler(10, (unsigned long)&__isr10);
    idt_install_handler(11, (unsigned long)&__isr11);
    idt_install_handler(12, (unsigned long)&__isr12);
    idt_install_handler(13, (unsigned long)&__isr13);
    idt_install_handler(14, (unsigned long)&__isr14);
    idt_install_handler(15, (unsigned long)&__isr15);
    idt_install_handler(16, (unsigned long)&__isr16);
    idt_install_handler(17, (unsigned long)&__isr17);
    idt_install_handler(18, (unsigned long)&__isr18);
    idt_install_handler(19, (unsigned long)&__isr19);
    idt_install_handler(20, (unsigned long)&__isr20);
    idt_install_handler(21, (unsigned long)&__isr21);

    // ISR 22-31 are reserved -- just use one common entry
    idt_install_handler(22, (unsigned long)&__isrReserved);
    idt_install_handler(23, (unsigned long)&__isrReserved);
    idt_install_handler(24, (unsigned long)&__isrReserved);
    idt_install_handler(25, (unsigned long)&__isrReserved);
    idt_install_handler(26, (unsigned long)&__isrReserved);
    idt_install_handler(27, (unsigned long)&__isrReserved);
    idt_install_handler(28, (unsigned long)&__isrReserved);
    idt_install_handler(29, (unsigned long)&__isrReserved);
    idt_install_handler(30, (unsigned long)&__isrReserved);
    idt_install_handler(31, (unsigned long)&__isrReserved);

    idt_install();
}
