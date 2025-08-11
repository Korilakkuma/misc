pub extern crate session_types as S;

pub type Put = S::Recv<u64, S::Recv<u64, S::Var<S::Z>>>;
pub type Get = S::Recv<u64, S::Send<Option<u64>, S::Var<S::Z>>>;

pub type DBServer = S::Rec<S::Offer<Put, S::Offer<Get, S::Eps>>>;
pub type DBClient = <DBServer as S::HasDual>::Dual;
