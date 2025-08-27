use std::sync::atomic::AtomicI32;
use std::sync::atomic::Ordering;

fn main() {
    let a = AtomicI32::new(100);
    let b = a.fetch_add(25, Ordering::Relaxed);
    let c = a.load(Ordering::Relaxed);

    assert_eq!(b, 100);
    assert_eq!(c, 125);
}
