#![cfg_attr(not(feature = "std"), no_std)]
//! Based on <https://github.com/RinLovesYou/dll-proxy-rs>.

#[cfg(feature = "alloc")]
extern crate alloc;

use core::{arch::global_asm, ffi::c_void};
#[cfg(windows)]
use windows::{
    core::{s as pcstr, PCSTR},
    Win32::{
        Foundation::{BOOL, FARPROC, HINSTANCE, HMODULE},
        System::{
            LibraryLoader::{FreeLibraryAndExitThread, GetProcAddress, LoadLibraryA},
            SystemServices::{
                DLL_PROCESS_ATTACH, DLL_PROCESS_DETACH, DLL_THREAD_ATTACH, DLL_THREAD_DETACH,
            },
        },
    },
};

#[no_mangle]
static mut OriginalFuncs_version: [FARPROC; 17] = [None; 17];

#[cfg(target_pointer_width = "64")]
global_asm!(include_str!("../deps/version.x64.S"));
#[cfg(target_pointer_width = "32")]
global_asm!(include_str!("../deps/version.x86.S"));

const EXPORTS_VERSION: [PCSTR; 17] = [
    pcstr!("GetFileVersionInfoA"),
    pcstr!("GetFileVersionInfoByHandle"),
    pcstr!("GetFileVersionInfoExA"),
    pcstr!("GetFileVersionInfoExW"),
    pcstr!("GetFileVersionInfoSizeA"),
    pcstr!("GetFileVersionInfoSizeExA"),
    pcstr!("GetFileVersionInfoSizeExW"),
    pcstr!("GetFileVersionInfoSizeW"),
    pcstr!("GetFileVersionInfoW"),
    pcstr!("VerFindFileA"),
    pcstr!("VerFindFileW"),
    pcstr!("VerInstallFileA"),
    pcstr!("VerInstallFileW"),
    pcstr!("VerLanguageNameA"),
    pcstr!("VerLanguageNameW"),
    pcstr!("VerQueryValueA"),
    pcstr!("VerQueryValueW"),
];

#[cfg(windows)]
#[no_mangle]
unsafe extern "system" fn DllMain(
    _inst_dll: HINSTANCE,
    reason: u32,
    _reserved: *mut c_void,
) -> BOOL {
    use std::thread;

    static mut ORIGINAL_DLL: Option<HMODULE> = None;
    static mut PAYLOAD_DLL: Option<HMODULE> = None;
    match reason {
        DLL_PROCESS_ATTACH => {
            ORIGINAL_DLL.replace(
                LoadLibraryA(pcstr!("C:\\Windows\\System32\\version.dll"))
                    .expect("couldn't load `version.dll`"),
            );
            if let Some(ref original_dll) = ORIGINAL_DLL {
                for (index, export) in EXPORTS_VERSION.iter().enumerate() {
                    OriginalFuncs_version[index] = GetProcAddress(*original_dll, *export);
                }
            }
            PAYLOAD_DLL.replace(LoadLibraryA(pcstr!("rusted_yoyo.dll")).expect("couldn't load `rusted-yoyo.dll`"));
            BOOL::from(true)
        }
        DLL_PROCESS_DETACH => {
            if let Some(original_dll) = ORIGINAL_DLL.take() {
                thread::spawn(move || FreeLibraryAndExitThread(original_dll, 0))
                    .join()
                    .expect("original DLL failed to unload and free");
            }
            if let Some(payload_dll) = ORIGINAL_DLL.take() {
                thread::spawn(move || FreeLibraryAndExitThread(payload_dll, 0))
                    .join()
                    .expect("payload DLL failed to unload and free");
            }
            BOOL::from(true)
        }
        DLL_THREAD_ATTACH => BOOL::from(true),
        DLL_THREAD_DETACH => BOOL::from(true),
        _ => BOOL::from(true),
    }
}
