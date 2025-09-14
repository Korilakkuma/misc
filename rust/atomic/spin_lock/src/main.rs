use std::sync::atomic::AtomicBool;
use std::sync::atomic::Ordering;
use std::thread;
use std::time::Duration;

pub struct SpinLock {
    locked: AtomicBool,
}

impl SpinLock {
    pub const fn new() -> Self {
        Self {
            locked: AtomicBool::new(false),
        }
    }

    pub fn lock(&self) {
        while self.locked.swap(true, Ordering::Acquire) {
            std::hint::spin_loop();
        }
    }

    pub fn unlock(&self) {
        self.locked.store(false, Ordering::Release);
    }
}

fn main() {
    static SPINLOCK: SpinLock = SpinLock::new();

    let th1 = thread::spawn(|| {
        SPINLOCK.lock();

        println!("thread 1 got Lock !");

        thread::sleep(Duration::from_secs(5));

        SPINLOCK.unlock();

        println!("thread 1 unlock !");
    });

    let th2 = thread::spawn(|| {
        SPINLOCK.lock();

        println!("thread 2 got Lock !");

        thread::sleep(Duration::from_secs(5));

        SPINLOCK.unlock();

        println!("thread 2 unlock !");
    });

    th1.join().unwrap();
    th2.join().unwrap();
}
