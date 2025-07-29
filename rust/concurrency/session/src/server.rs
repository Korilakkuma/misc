use crate::channel::{Server, S};

pub fn server(c: S::Chan<(), Server>) {
    let (c, n) = c.recv();

    match c.offer() {
        S::Branch::Left(c) => {
            c.send(n * n).close();
        }
        S::Branch::Right(c) => {
            c.send(n & 1 == 0).close();
        }
    }
}
