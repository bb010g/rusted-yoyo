use anyhow::Context;
use std::path::Path;

fn main() -> anyhow::Result<()> {
    let mut bindings_builder = bindgen::builder()
        .header("src/gm.hpp")
        .clang_arg("-std=c++20")
        .use_core()
        .default_enum_style(bindgen::EnumVariation::NewType {
            is_bitfield: false,
            is_global: false,
        })
        // .layout_tests(false)
        .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()));
    bindings_builder = bindings_builder.emit_diagnostics();
    let bindings = bindings_builder
        .generate()
        .context("Couldn't generate bindings")?;
    bindings
        .write_to_file(Path::new("src/gm.rs"))
        .context("Couldn't write bindings")?;
    Ok(())
}
