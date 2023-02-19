#pragma once

/**
 * Initialise the physical allocator system
 *
 * This function gets called during the early setup process to initialise
 * the physical frame allocator -- which is needed for the rest of the
 * memory management subsystem to function.
 *
 * See arch/x86_64/boot/boot.S for invocation details
 */
void arch_phys_init(void);
