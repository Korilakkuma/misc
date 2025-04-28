use std::sync::{Arc, Barrier};
use std::thread;

fn main() {
    let number_of_thread = 10;

    let barriers = Arc::new(Barrier::new(number_of_thread));

    let mut handlers = vec![];

    for id in 0..number_of_thread {
        let barrier = barriers.clone();

        let handler = thread::spawn(move || {
            barrier.wait();

            println!("thread id[{}]: Synchronized Barrier", id)
        });

        handlers.push(handler);
    }

    let mut id: usize = 0;

    for handler in handlers {
        match handler.join() {
            Ok(_) => {
                println!("thread id[{}]: Ended", id);
                id += 1;
            }
            Err(_) => panic!("thread id[{}]: Panic", id),
        }
    }
}
