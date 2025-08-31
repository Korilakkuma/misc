use std::sync::atomic::{AtomicI32, Ordering};
use std::thread;

static X: AtomicI32 = AtomicI32::new(0);

fn main() {
    let t_a_1 = thread::spawn(|| a_1());
    let t_a_2 = thread::spawn(|| a_2());
    let t_b = thread::spawn(|| b());

    t_a_1.join().unwrap();
    t_a_2.join().unwrap();
    t_b.join().unwrap();
}

fn a_1() {
    X.fetch_add(5, Ordering::Relaxed);
}

fn a_2() {
    X.fetch_add(10, Ordering::Relaxed);
}

fn b() {
    let a = X.load(Ordering::Relaxed);
    let b = X.load(Ordering::Relaxed);
    let c = X.load(Ordering::Relaxed);
    let d = X.load(Ordering::Relaxed);

    println!("{a} {b} {c} {d}");
}
