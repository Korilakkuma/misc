use std::sync::atomic::{AtomicBool, Ordering};
use std::thread;

static A: AtomicBool = AtomicBool::new(false);
static B: AtomicBool = AtomicBool::new(false);

static mut S: String = String::new();

fn main() {
    let th1 = thread::spawn(|| {
        A.store(true, Ordering::SeqCst);

        if !B.load(Ordering::SeqCst) {
            unsafe { S.push('!') }
        }
    });

    let th2 = thread::spawn(|| {
        B.store(true, Ordering::SeqCst);

        if !A.load(Ordering::SeqCst) {
            unsafe { S.push('!') }
        }
    });

    th1.join().unwrap();
    th2.join().unwrap();
}
