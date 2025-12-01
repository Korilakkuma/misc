use std::sync::atomic::{AtomicI32, Ordering};

fn main() {
    let n = AtomicI32::new(32);
    let r = x86_64(&n);

    println!("x86-64 compare and exchange: {}", r);
}

fn x86_64(x: &AtomicI32) -> i32 {
    let mut current = x.load(Ordering::Relaxed);

    loop {
        let new = current | 10;

        match x.compare_exchange(current, new, Ordering::Relaxed, Ordering::Relaxed) {
            Ok(v) => return v,
            Err(v) => current = v,
        }
    }
}
