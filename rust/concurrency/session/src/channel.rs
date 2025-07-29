pub extern crate session_types as S;

pub type Client = S::Send<u64, S::Choose<S::Recv<u64, S::Eps>, S::Recv<bool, S::Eps>>>;
pub type Server = <Client as S::HasDual>::Dual;

pub enum Op {
    Square,
    Even,
}
