use crate::channel::{S, DBCLient};

fn db_client(c: S::Chan<(), DBClient>) {
    let c = c.enter();

    let c = c.sel1().send(10).send(40).zero();
    let c = c.sel1().send(50).send(7).zero();
}
