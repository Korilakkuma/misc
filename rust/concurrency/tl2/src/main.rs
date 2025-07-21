mod tl2;

use std::sync::Arc;
use std::{thread, time};

#[macro_export]
macro_rules! load {
    ($t:ident, $a:expr) => {
        if let Some(v) = ($t).load($a) {
            v
        } else {
            return tl2::STMResult::Retry;
        }
    };
}

#[macro_export]
macro_rules! store {
    ($t:ident, $a:expr, $v:expr) => {
        $t.store($a, $v)
    };
}

const NUMBER_OF_THREADS: usize = 8;

fn main() {}

fn thread_executor(stm: Arc<tl2::STM>, n: usize) {
    let left = 8 * n;
    let right = 8 * ((n + 1) % NUMBER_OF_THREADS);

    for _ in 0..500000 {
        while !stm
            .write_transaction(|tr| {
                let mut f1 = load!(tr, left);
                let mut f2 = load!(tr, right);

                if f1[0] == 0 && f2[0] == 0 {
                    f1[0] = 1;
                    f2[0] = 1;

                    store!(tr, left, f1);
                    store!(tr, right, f2);

                    tl2::STMResult::Ok(true)
                } else {
                    tl2::STMResult::Ok(false)
                }
            })
            .unwrap()
        { /* spin lock */ }

        stm.write_transaction(|tr| {
            let mut f1 = load!(tr, left);
            let mut f2 = load!(tr, right);

            f1[0] = 0;
            f2[0] = 0;

            store!(tr, left, f1);
            store!(tr, right, f2);

            tl2::STMResult::Ok(())
        });
    }
}

fn observer(stm: Arc<tl2::STM>) {
    for _ in 0..10000 {
        let resource = stm
            .read_transaction(|tr| {
                let mut v = [0; NUMBER_OF_THREADS];

                for i in 0..NUMBER_OF_THREADS {
                    v[i] = load!(tr, 8 * i)[0];
                }

                tl2::STMResult::Ok(v)
            })
            .unwrap();

        println!("{:?}", resource);

        let mut n = 0;

        for r in &resource {
            if *r == 1 {
                n += 1;
            }
        }

        if n & 1 != 0 {
            panic!("inconsistent");
        }
    }

    let us = time::Duration::from_micros(100);

    thread::sleep(us);
}
