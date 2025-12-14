use std::sync::atomic::{compiler_fence, AtomicBool, AtomicUsize, Ordering};
use std::thread;

fn main() {
    let locked = AtomicBool::new(false);
    let counter = AtomicUsize::new(0);

    thread::scope(|s| {
        for _ in 0..4 {
            s.spawn(|| {
                for _ in 0..1_000_000 {
                    while locked.swap(true, Ordering::Relaxed) {}

                    compiler_fence(Ordering::Acquire);

                    let old = counter.load(Ordering::Relaxed);
                    let new = old + 1;

                    counter.store(new, Ordering::Relaxed);

                    compiler_fence(Ordering::Release);

                    locked.store(false, Ordering::Relaxed);
                }
            });
        }
    });

    println!("{}", counter.into_inner());
}
