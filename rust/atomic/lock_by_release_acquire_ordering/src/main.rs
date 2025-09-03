use std::sync::atomic::{AtomicBool, Ordering};
use std::thread;

static mut DATA: String = String::new();
static LOCKED: AtomicBool = AtomicBool::new(false);

fn main() {
    thread::scope(|s| {
        for _ in 0..100 {
            s.spawn(thread_func);
        }
    });
}

fn thread_func() {
    if LOCKED
        .compare_exchange(false, true, Ordering::Acquire, Ordering::Relaxed)
        .is_ok()
    {
        unsafe { DATA.push('!') }

        LOCKED.store(false, Ordering::Release);
    }
}
