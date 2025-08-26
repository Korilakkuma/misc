use std::sync::atomic::AtomicU64;
use std::sync::atomic::Ordering;
use std::thread;

fn main() {
    let th1 = thread::spawn(|| get_u64());

    let th2 = thread::spawn(|| get_u64());

    let x1 = th1.join().unwrap();
    let x2 = th2.join().unwrap();

    println!("x1 is {}", x1);
    println!("x2 is {}", x2);
}

fn get_u64() -> u64 {
    static X: AtomicU64 = AtomicU64::new(0);

    let mut x = X.load(Ordering::Relaxed);

    if x == 0 {
        x = 100;

        X.store(x, Ordering::Relaxed);
    }

    x
}
