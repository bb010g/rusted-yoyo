#![cfg_attr(not(feature = "std"), no_std)]
#[cfg(feature = "alloc")]
extern crate alloc;

use core::ffi::c_void;
use std::error::Error;
#[cfg(windows)]
use windows::Win32::{
    Foundation::{BOOL, HINSTANCE},
    System::SystemServices::{
        DLL_PROCESS_ATTACH, DLL_PROCESS_DETACH, DLL_THREAD_ATTACH, DLL_THREAD_DETACH,
    },
};

fn hook() -> Result<(), Box<dyn Error + Send>> {
    // // Retrieve an absolute address of `MessageBoxW`. This is required for
    // // libraries due to the import address table. If `MessageBoxW` would be
    // // provided directly as the target, it would only hook this DLL's
    // // `MessageBoxW`. Using the method below an absolute address is retrieved
    // // instead, detouring all invocations of `MessageBoxW` in the active process.
    // let address = get_module_symbol_address("user32.dll", "MessageBoxW")
    //   .expect("could not find 'MessageBoxW' address");
    // let target: FnMessageBoxW = mem::transmute(address);

    // // Initialize AND enable the detour (the 2nd parameter can also be a closure)
    // MessageBoxWHook
    //   .initialize(target, messageboxw_detour)?
    //   .enable()?;

    Ok(())
}

fn unhook() -> Result<(), Box<dyn Error + Send>> {
    Ok(())
}

#[cfg_attr(not(windows), ctor::ctor)]
fn hook_main() {
    hook().expect("hook")
}

// #[cfg_attr(not(windows), ctor::dtor)]
fn unhook_main() {
    unhook().expect("unhook")
}

#[cfg(windows)]
#[no_mangle]
unsafe extern "system" fn DllMain(
    _inst_dll: HINSTANCE,
    reason: u32,
    _reserved: *mut c_void,
) -> BOOL {
    use std::thread;

    static mut MAIN_THREAD: Option<thread::JoinHandle<()>> = None;
    match reason {
        DLL_PROCESS_ATTACH => {
            if let Some(main_thread) = MAIN_THREAD.replace(thread::spawn(hook_main)) {
                main_thread.join().expect("main thread failed to join");
            }
            BOOL::from(true)
        }
        DLL_PROCESS_DETACH => {
            if let Some(main_thread) = MAIN_THREAD.take() {
                main_thread.join().expect("main thread failed to join");
            }
            unhook_main();
            BOOL::from(true)
        }
        DLL_THREAD_ATTACH => BOOL::from(true),
        DLL_THREAD_DETACH => BOOL::from(true),
        _ => BOOL::from(true),
    }
}
