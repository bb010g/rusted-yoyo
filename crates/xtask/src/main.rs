#![cfg_attr(not(feature = "std"), no_std)]
#[cfg(feature = "alloc")]
extern crate alloc;

use bpaf::Bpaf;
use cargo_util::ProcessBuilder;
use color_eyre::eyre;
use core::str::FromStr;
use derive_more::Display;
use std::ffi::OsString;
use thiserror::Error;

pub(crate) fn verbosity<'a, T>(present: &'a T, step: &'a T) -> impl 'a + bpaf::Parser<T>
where
    T: num_traits::CheckedAdd + num_traits::CheckedSub + Clone,
{
    use bpaf::Parser;
    bpaf::construct!([
        v(bpaf::short('v')
            .long("verbose")
            .help("Increases output verbosity")
            .req_flag(true)),
        q(bpaf::short('q')
            .long("quiet")
            .help("Decreases output verbosity")
            .req_flag(false)),
    ])
    .try_fold_with(move || {
        (present.clone(), move |acc: T, sign| {
            if sign {
                acc.checked_add(&step).ok_or("verbosity overflow")
            } else {
                acc.checked_sub(&step).ok_or("verbosity underflow")
            }
        })
    })
}

#[derive(Bpaf, Clone, Debug)]
#[bpaf(options("xtask"))]
pub struct Options {
    /// Output verbosity
    #[bpaf(external(verbosity(&0, &127)))]
    #[allow(dead_code)]
    verbosity: u8,

    #[bpaf(external(options_command()))]
    command: OptionsCommand,
}

#[derive(Bpaf, Clone, Debug)]
pub enum OptionsCommand {
    Bin(#[bpaf(external(bin_options()))] BinOptions),
}

#[derive(Bpaf, Clone, Debug)]
#[bpaf(command("bin"))]
pub struct BinOptions {
    #[bpaf(external(bin_options_command()))]
    command: BinOptionsCommand,
    #[bpaf(positional("ARGS"))]
    args: Vec<OsString>,
}

pub fn bin_options_command() -> impl bpaf::Parser<BinOptionsCommand> {
    use bpaf::Parser;
    bpaf::construct!([
        bindgen(bpaf::literal("bindgen").map(|()| BinOptionsCommand::Bindgen)),
        msrv(bpaf::literal("msrv").map(|()| BinOptionsCommand::Msrv)),
        nono(bpaf::literal("nono").map(|()| BinOptionsCommand::Nono)),
    ])
}
#[derive(Copy, Clone, Debug, Display, Eq, PartialEq)]
pub enum BinOptionsCommand {
    #[display(fmt = "bindgen")]
    Bindgen,
    #[display(fmt = "msrv")]
    Msrv,
    #[display(fmt = "nono")]
    Nono,
}
impl FromStr for BinOptionsCommand {
    type Err = BinOptionsCommandParseError;
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "bindgen" => Ok(Self::Bindgen),
            "msrv" => Ok(Self::Msrv),
            "nono" => Ok(Self::Nono),
            _ => Err(Self::Err::UnexpectedString),
        }
    }
}
#[derive(Copy, Clone, Debug, Eq, Error, PartialEq)]
#[non_exhaustive]
pub enum BinOptionsCommandParseError {
    #[error("provided string was not \"bindgen\", \"msrv\", or \"nono\"")]
    UnexpectedString,
}

pub fn main_with_options(options: Options) -> Result<(), eyre::Report> {
    match options.command {
        OptionsCommand::Bin(bin_options) => {
            let program = match bin_options.command {
                BinOptionsCommand::Bindgen => env!("CARGO_BIN_FILE_BINDGEN_CLI_BINDGEN"),
                BinOptionsCommand::Msrv => env!("CARGO_BIN_FILE_CARGO_MSRV"),
                BinOptionsCommand::Nono => env!("CARGO_BIN_FILE_CARGO_DAVINONO"),
            };
            let mut process = ProcessBuilder::new(program);
            process.args(&bin_options.args);
            if options.verbosity >= 2 {
                process.display_env_vars();
            }
            process.exec_replace().map_err(eyre::Report::msg)?;
        }
    }

    Ok(())
}

pub fn main() -> Result<(), eyre::Report> {
    color_eyre::install()?;

    main_with_options(options().run())
}
