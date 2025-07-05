mod fairlock;

use std::sync::Arc;

const NUMBER_OF_LOOPS: usize = 100000;
const NUMBER_OF_THREADS: usize = 8;

fn main() {
    let lock = Arc::new(fairlock::FairLock::new(0));

    let mut handlers = vec![];

    for index in 0..NUMBER_OF_THREADS {
        let lock_0 = lock.clone();

        let handler = std::thread::spawn(move || {
            for _ in 0..NUMBER_OF_LOOPS {
                let mut data = lock_0.lock(index);

                *data += 1;
            }
        });

        handlers.push(handler);
    }

    for handler in handlers {
        handler.join().unwrap();
    }

    println!(
        "Count = {} (Expected = {})",
        *lock.lock(0),
        (NUMBER_OF_LOOPS * NUMBER_OF_THREADS)
    );
}
