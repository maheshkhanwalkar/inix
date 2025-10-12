use std::env;

fn main() {
    let target_arch = env::var("CARGO_CFG_TARGET_ARCH").unwrap();
    match target_arch.as_str() {
        "aarch64" => { aarch64_setup(); }
        _ => panic!("Unsupported architecture"),
    }
}

fn aarch64_setup() {
    println!("cargo:rerun-if-changed=src/arch/arm64");
    println!("cargo::rustc-link-arg=-Tsrc/arch/arm64/linker.ld");

    // FIXME: if we add more files, need to update this!
    cc::Build::new()
        .file("src/arch/arm64/entry.S")
        .compile("entry");
}
