use esp_idf_svc::sys::link_patches;

#[no_mangle]
extern "C" fn app_main() {
    link_patches();

    let result = peripheral_bindings::add(20, 30);

    println!("result is {}", result);
}
