#[macro_use]
extern crate session_types;

pub use session_types as S;

pub type Client = S::Send<u64, S::Choose<S::Recv<u64, S::Eps>, S::Recv<bool, S::Eps>>>;
pub type Server = <Client as S::HasDual>::Dual;

pub type Put = S::Recv<u64, S::Recv<u64, S::Var<S::Z>>>;
pub type Get = S::Recv<u64, S::Send<Option<u64>, S::Var<S::Z>>>;

pub type DBServer = S::Rec<S::Offer<Put, S::Offer<Get, S::Eps>>>;
pub type DBCLient = <DBServer as S::HasDual>::Dual;

pub enum Op {
    Square,
    Even,
}
