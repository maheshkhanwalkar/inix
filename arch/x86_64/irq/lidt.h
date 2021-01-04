#pragma once

// See arch/x86_64/irq/lidt.S for implementation
void _lidt(void* idtr);
