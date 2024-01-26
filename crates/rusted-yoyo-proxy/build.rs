use std::path::Path;

fn main() {
    let lib_path = Path::new("deps").join("exports.def");
    let canon_lib_path = std::fs::canonicalize(lib_path).expect("`deps/exports.def` has canonical form");
    println!(
        "cargo:rustc-cdylib-link-arg=/DEF:{}",
        canon_lib_path.display()
    );
}
