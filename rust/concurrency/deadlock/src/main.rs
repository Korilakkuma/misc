use std::sync::{Arc, Mutex};
use std::thread;

const NMUBER_OF_LOOPS: u64 = 10;

fn main() {
    let thread_0_resource_0 = Arc::new(Mutex::new(()));
    let thread_0_resource_1 = Arc::new(Mutex::new(()));

    let thread_1_resource_0 = thread_0_resource_0.clone();
    let thread_1_resource_1 = thread_0_resource_1.clone();

    println!("Will cause Deadlock !");

    let thread_0 = thread::spawn(move || {
        for _ in 0..NMUBER_OF_LOOPS {
            let _lock_0 = thread_0_resource_0.lock().unwrap();

            println!("thread 0: Get Lock 0");

            let _lock_1 = thread_0_resource_1.lock().unwrap();

            println!("thread 0: Get Lock 1");
        }
    });

    let thread_1 = thread::spawn(move || {
        for _ in 0..NMUBER_OF_LOOPS {
            let _lock_1 = thread_1_resource_1.lock().unwrap();

            println!("thread 1: Get Lock 1");

            let _lock_0 = thread_1_resource_0.lock().unwrap();

            println!("thread 1: Get Lock 0");

            // correct to the following
            // let _lock_0 = thread_1_resource_0.lock().unwrap();

            // println!("thread 1: Get Lock 0");

            // let _lock_1 = thread_1_resource_1.lock().unwrap();

            // println!("thread 1: Get Lock 1");
        }
    });

    thread_0.join().unwrap();
    thread_1.join().unwrap();

    println!("thread 0 and thread 1 were ended")
}
