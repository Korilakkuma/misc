use crate::channel::{DBServer, S};
use std::collections::HashMap;

pub fn db_server(c: S::Chan<(), DBServer>) {
    let mut c_enter = c.enter();

    let mut db = HashMap::new();

    loop {
        match c_enter.offer() {
            S::Branch::Left(c) => {
                let (c, key) = c.recv();
                let (c, val) = c.recv();

                db.insert(key, val);

                c_enter = c.zero();
            }
            S::Branch::Right(c) => match c.offer() {
                S::Branch::Left(c) => {
                    let (c, key) = c.recv();

                    let c = if let Some(val) = db.get(&key) {
                        c.send(Some(*val))
                    } else {
                        c.send(None)
                    };

                    c_enter = c.zero();
                }
                S::Branch::Right(c) => {
                    c.close();
                    return;
                }
            },
        }
    }
}
