#![no_std]
#![no_main]

// Arch mods
#[cfg(target_arch = "aarch64")] #[path="arch/arm64/mod.rs"]
mod arch;

// Common modules
mod panic;

#[unsafe(no_mangle)]
pub fn kernel_main() {
    // TODO
    loop {
    }
}
