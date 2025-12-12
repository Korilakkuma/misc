use std::hint::black_box;
use std::sync::atomic::{AtomicU64, Ordering};
use std::thread;
use std::time::Instant;

static A: [AtomicU64; 3] = [AtomicU64::new(0), AtomicU64::new(0), AtomicU64::new(0)];

fn main() {
    black_box(&A);

    thread::spawn(|| loop {
        A[0].store(0, Ordering::Relaxed);
        A[2].store(0, Ordering::Relaxed);
    });

    let start = Instant::now();

    for _ in 0..1_000_000_000 {
        black_box(A[1].load(Ordering::Relaxed));
    }

    println!("{:?}", start.elapsed());
}
