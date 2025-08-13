use crate::channel::{DBClient, S};

pub fn db_client(c: S::Chan<(), DBClient>) {
    let c = c.enter();

    // Put
    let c = c.sel1().send(10).send(4).zero();
    let c = c.sel1().send(50).send(7).zero();

    // Get
    let (c, val) = c.sel2().sel1().send(10).recv();

    println!("val = {:?}", val);

    let c = c.zero();

    // Get
    let (c, val) = c.sel2().sel1().send(20).recv();

    println!("val = {:?}", val);

    let _ = c.zero().sel2().sel2().close();
}
