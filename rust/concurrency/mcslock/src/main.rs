mod mcslock;

use std::sync::Arc;

const NUMBER_OF_LOOPS: usize = 100000;
const NUMBER_OF_THREADS: usize = 4;

fn main() {
    let n = Arc::new(mcslock::MCSLock::new(0));

    let mut handlers = vec![];

    for _ in 0..NUMBER_OF_THREADS {
        let n0 = n.clone();

        let handler = std::thread::spawn(move || {
            let mut node = mcslock::MCSNode::new();

            for _ in 0..NUMBER_OF_LOOPS {
                let mut v = n0.lock(&mut node);

                *v += 1;
            }
        });

        handlers.push(handler);
    }

    for handler in handlers {
        handler.join().unwrap();
    }

    let mut node = mcslock::MCSNode::new();
    let l = n.lock(&mut node);

    println!(
        "Count = {} (Expected = {})",
        *l,
        (NUMBER_OF_LOOPS * NUMBER_OF_THREADS)
    );
}
