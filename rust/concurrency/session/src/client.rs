use crate::channel::{Client, Op, S};

pub fn client(c: S::Chan<(), Client>, n: u64, op: Op) {
    let c = c.send(n);

    match op {
        Op::Square => {
            let c = c.sel1();

            let (c, val) = c.recv();

            c.close();

            println!("{}^2 = {}", n, val);
        }
        Op::Even => {
            let c = c.sel2();

            let (c, val) = c.recv();

            c.close();

            if val {
                println!("{} is even", n);
            } else {
                println!("{} is odd", n);
            }
        }
    }
}
