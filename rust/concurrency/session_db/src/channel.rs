pub extern crate session_types as S;

pub type Put = S::Recv<u64, S::Recv<u64, S::Var<S::Z>>>;
pub type Get = S::Recv<u64, S::Send<Option<u64>, S::Var<S::Z>>>;

pub type DBServer = S::Rec<S::Offer<Put, S::Offer<Get, S::Eps>>>;
pub type DBClient = <DBServer as S::HasDual>::Dual;

pub type SChan = S::Chan<(), S::Send<(), S::Eps>>;
pub type ChanRecv = S::Recv<SChan, S::Eps>;
pub type ChanSend = <ChanRecv as S::HasDual>::Dual;

pub fn chan_recv(c: S::Chan<(), ChanRecv>) {
    let (c, cr) = c.recv();

    c.close();

    let cr = cr.send(());

    cr.close();
}

pub fn chan_send(c: S::Chan<(), ChanSend>) {
    let (c1, c2) = S::session_channel();

    let c = c.send(c1);

    c.close();

    let (c2, _) = c2.recv();

    c2.close();
}
