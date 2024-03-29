use std::thread;
use std::time::Duration;

fn main() {
    let handler = thread::spawn(|| {
        for i in 1..10 {
            println!("number {} from the spawned thread !", i);
            thread::sleep(Duration::from_millis(1));
        }
    });

    for i in 1..5 {
        println!("number {} from the main thread !", i);
        thread::sleep(Duration::from_millis(1));
    }

    handler.join().unwrap();
}
