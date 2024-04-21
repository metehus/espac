use std::env;
use std::path::PathBuf;
use bindgen::Builder;

fn main() {
    println!("cargo:rustc-link-search=../../.pio/libdeps/release");

    embuild::espidf::sysenv::output();
    let bindings = embuild::bindgen::run(
        Builder::default()
            .header("bindings.h")
    ).expect("error!");
    // Tell cargo to look for shared libraries in the specified directory
    // println!("cargo:rustc-link-search=/path/to/lib");

    // Tell cargo to tell rustc to link the system bzip2
    // shared library.
    // println!("cargo:rustc-link-lib=bz2");

    // The bindgen::Builder is the main entry point
    // to bindgen, and lets you build up options for
    // the resulting bindings.
    // let bindings = bindgen::Builder::default()
    //     // The input header we would like to generate
    //     // bindings for.
    //     .header("wrapper.h")
    //     // Tell cargo to invalidate the built crate whenever any of the
    //     // included header files changed.
    //     .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))
    //     // Finish the builder and generate the bindings.
    //     .generate()
    //     // Unwrap the Result and panic on failure.
    //     .expect("Unable to generate bindings");

    // Write the bindings to the $OUT_DIR/bindings.rs file.
}