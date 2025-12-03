use std::hint::black_box;
use std::sync::atomic::{AtomicU64, Ordering};
use std::time::Instant;

static A: AtomicU64 = AtomicU64::new(0);

fn main() {
    black_box(&A);

    let start = Instant::now();

    for _ in 0..1_000_000_000 {
        black_box(A.load(Ordering::Relaxed));
    }

    println!("{:?}", start.elapsed());
}
