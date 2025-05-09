use std::sync::{Arc, RwLock};
use std::thread;

fn main() {
    let rwlock = Arc::new(RwLock::new(true));

    let handler = thread::spawn(move || {
        let flag = rwlock.read().unwrap();

        println!("Will cause Deadlock !");

        if *flag {
            println!("Reader is acquiring Lock");
            println!("Writer cannot acquire Lock");

            let mut value = rwlock.write().unwrap();

            *value = false;

            println!("Ended {}", *value);
        }

        // correct to the following
        // let flag = *rwlock.read().unwrap();

        // println!("Reader released Lock");

        // if flag {
        //     println!("Writer acquire Lock");

        //     let mut value = rwlock.write().unwrap();

        //     *value = false;

        //     println!("Ended {}", *value);
        // }
    });

    handler.join().unwrap();
}
