use std::sync::atomic::{AtomicI32, Ordering};
use std::thread;
use std::thread::sleep;
use std::time::Duration;

static X: AtomicI32 = AtomicI32::new(0);
static Y: AtomicI32 = AtomicI32::new(0);

fn main() {
    let t_a = thread::spawn(|| a());
    let t_b = thread::spawn(|| b());

    t_a.join().unwrap();
    t_b.join().unwrap();
}

fn a() {
    X.store(10, Ordering::Relaxed);

    sleep(Duration::from_secs(1));

    Y.store(20, Ordering::Relaxed);
}

fn b() {
    let y = Y.load(Ordering::Relaxed);
    let x = X.load(Ordering::Relaxed);

    println!("{x} {y}");
}
