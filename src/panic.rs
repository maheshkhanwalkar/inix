
#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    // do nothing for now
    loop {}
}
