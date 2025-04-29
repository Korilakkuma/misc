use semaphore::Semaphore;
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::Arc;
use std::thread;

const NUMBER_OF_LOOPS: usize = 3;
const NUMBER_OF_THREADS: usize = 8;
const NUMBER_OF_SEMAPHORES: isize = 4;

static COUNT: AtomicUsize = AtomicUsize::new(0);

fn main() {
    let mut handlers = vec![];

    let semaphore = Arc::new(Semaphore::new(NUMBER_OF_SEMAPHORES));

    for id in 0..NUMBER_OF_THREADS {
        let sem = semaphore.clone();

        let handler = thread::spawn(move || {
            for _ in 0..NUMBER_OF_LOOPS {
                sem.wait();

                COUNT.fetch_add(1, Ordering::SeqCst);

                let n = COUNT.load(Ordering::SeqCst);

                assert!((n as isize) <= NUMBER_OF_SEMAPHORES);

                println!("thread id[{}]: count = {}", id, n);

                COUNT.fetch_sub(1, Ordering::SeqCst);

                sem.post();
            }
        });

        handlers.push(handler);
    }

    let mut id: usize = 0;

    for handler in handlers {
        match handler.join() {
            Ok(_) => {
                println!("thread id [{}]: Ended", id);

                id += 1;
            }
            Err(_) => panic!("thread id [{}]: Panic", id),
        }
    }
}
