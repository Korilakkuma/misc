use std::sync::atomic::AtomicBool;
use std::sync::atomic::Ordering;
use std::thread;
use std::time::Duration;

static mut DATA: u64 = 0;
static READY: AtomicBool = AtomicBool::new(false);

fn main() {
    thread::spawn(|| {
        unsafe { DATA = 11 }
        READY.store(true, Ordering::Release);
    });

    while !READY.load(Ordering::Acquire) {
        thread::sleep(Duration::from_millis(100));

        println!("Waiting ...");
    }

    println!("{}", unsafe { DATA });
}
