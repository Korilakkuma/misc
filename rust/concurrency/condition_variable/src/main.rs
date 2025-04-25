use std::sync::{Arc, Condvar, Mutex};
use std::thread;

fn main() {
    let pair_0 = Arc::new((Mutex::new(false), Condvar::new()));

    let pair_1 = pair_0.clone();
    let pair_2 = pair_0.clone();

    let consumber_1 = thread::spawn(move || {
        consume(1, pair_1);
    });

    let consumber_2 = thread::spawn(move || {
        consume(2, pair_2);
    });

    let producer = thread::spawn(move || {
        produce(0, pair_0);
    });

    consumber_1.join().unwrap();
    consumber_2.join().unwrap();
    producer.join().unwrap();
}

fn produce(id: u64, p: Arc<(Mutex<bool>, Condvar)>) {
    let &(ref lock, ref cvar) = &*p;

    match lock.lock() {
        Ok(mut ready) => {
            *ready = true;

            cvar.notify_all();

            println!("producer thread id [{}]", id);
        }
        Err(error) => panic!("{}", error),
    }
}

fn consume(id: u64, p: Arc<(Mutex<bool>, Condvar)>) {
    let &(ref lock, ref cvar) = &*p;

    match lock.lock() {
        Ok(mut ready) => {
            while !(*ready) {
                ready = cvar.wait(ready).unwrap();
            }

            // cvar.wait_while(ready, |ready| !(*ready)).unwrap();

            println!("consumer thread id [{}]", id);
        }
        Err(error) => panic!("{}", error),
    }
}
