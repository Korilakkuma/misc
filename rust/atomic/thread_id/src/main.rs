use std::thread;
use std::thread::Builder;

fn main() {
    let t1 = Builder::new().spawn(thread_func).unwrap();
    let t2 = Builder::new().spawn(thread_func).unwrap();

    // thread::spawn(thread_func);
    // thread::spawn(thread_func);

    println!("main thread has done");

    t1.join().unwrap();
    t2.join().unwrap();
}

fn thread_func() {
    println!("Thread Start !");

    let id = thread::current().id();

    println!("Current Thread ID: {id:?}");
}
