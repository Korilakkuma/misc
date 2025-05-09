use std::sync::RwLock;

fn main() {
    let rwlock = RwLock::new(true);

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
}
