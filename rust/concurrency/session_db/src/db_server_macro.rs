use crate::channel::{DBServer, S};
use std::collections::HashMap;

pub fn db_server_macro(c: S::Chan<(), DBServer>) {
    let mut c_enter = c.enter();
    let mut db = HashMap::new();

    loop {
        let c = c_enter;

        offer! {c,
            Put => {
                let (c, key) = c.recv();
                let (c, val) = c.recv();

                db.insert(key, val);

                c_enter = c.zero();
            },
            Get => {
                let (c, key) = c.recv();

                let c = if let Some(val) = db.get(&key) {
                    c.send(Some(*val))
                } else {
                    c.send(None)
                };

                c_enter = c.zero();
            },
            Quit => {
                c.close();
                return;
            }
        }
    }
}
