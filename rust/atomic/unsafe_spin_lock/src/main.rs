use std::cell::UnsafeCell;
use std::marker::{Send, Sync};
use std::sync::atomic::AtomicBool;
use std::sync::atomic::Ordering;
use std::thread;
use std::time::Duration;

pub struct SpinLock<T> {
    locked: AtomicBool,
    value: UnsafeCell<T>,
}

unsafe impl<T> Sync for SpinLock<T> where T: Send {}

impl<T> SpinLock<T> {
    pub const fn new(value: T) -> Self {
        Self {
            locked: AtomicBool::new(false),
            value: UnsafeCell::new(value),
        }
    }

    pub fn lock<'a>(&'a self) -> &'a mut T {
        while self.locked.swap(true, Ordering::Acquire) {
            std::hint::spin_loop();
        }

        unsafe { &mut *self.value.get() }
    }

    pub unsafe fn unlock(&self) {
        self.locked.store(false, Ordering::Release);
    }
}

fn main() {
    static SPINLOCK: SpinLock<isize> = SpinLock::new(5);

    let th1 = thread::spawn(|| {
        SPINLOCK.lock();

        println!("thread 1 got Lock !");

        thread::sleep(Duration::from_secs(5));

        unsafe { SPINLOCK.unlock() }

        println!("thread 1 unlock !");
    });

    let th2 = thread::spawn(|| {
        SPINLOCK.lock();

        println!("thread 2 got Lock !");

        thread::sleep(Duration::from_secs(5));

        unsafe { SPINLOCK.unlock() }

        println!("thread 2 unlock !");
    });

    th1.join().unwrap();
    th2.join().unwrap();
}
