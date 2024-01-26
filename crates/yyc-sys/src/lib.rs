#![allow(non_snake_case, non_camel_case_types)]
#![cfg_attr(not(feature = "std"), no_std)]
#[cfg(feature = "alloc")]
extern crate alloc;

// use core::ffi::CStr;

// include!("gm.rs");
mod gm;
pub use gm::*;
