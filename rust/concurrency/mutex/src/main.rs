use std::sync::{Arc, Mutex};
use std::thread;

fn main() {
    let lock_0 = Arc::new(Mutex::new(0));

    let mut threads = vec![];

    for id in 0..10 {
        let lock = lock_0.clone();

        let thread = thread::spawn(move || {
            thread_func(lock, id);
        });

        threads.push(thread);
    }

    let mut i = 0;

    for thread in threads {
        match thread.join() {
            Ok(_) => {
                println!("thread id[{}]: Success !", i);
                i += 1;
            }
            Err(_) => panic!("Error"),
        }
    }
}

fn thread_func(lock: Arc<Mutex<u64>>, thread_id: usize) {
    loop {
        // let mut value = lock.lock().unwrap();
        match lock.lock() {
            Ok(mut value) => {
                if *value > 1000 {
                    break;
                }

                *value += 1;

                println!("thread id[{}]: {}", thread_id, *value);
            }
            Err(error) => panic!("{}", error),
        }
    }
}
