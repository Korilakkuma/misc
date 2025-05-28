use std::cell::UnsafeCell;
use std::marker::{Send, Sync};
use std::ops::{Deref, DerefMut, Drop};
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;
use std::thread;

const NUMBER_OF_THREADS: usize = 4;
const NUMBER_OF_LOOPS: usize = 1000000;

struct SpinLock<T> {
    lock: AtomicBool,
    data: UnsafeCell<T>,
}

struct SpinLockGuard<'a, T> {
    spin_lock: &'a SpinLock<T>,
}

impl<T> SpinLock<T> {
    fn new(v: T) -> Self {
        SpinLock {
            lock: AtomicBool::new(false),
            data: UnsafeCell::new(v),
        }
    }

    fn lock(&self) -> SpinLockGuard<T> {
        loop {
            while self.lock.load(Ordering::Relaxed) {}

            if let Ok(_) =
                self.lock
                    .compare_exchange_weak(false, true, Ordering::Acquire, Ordering::Relaxed)
            {
                break;
            }
        }

        SpinLockGuard { spin_lock: self }
    }
}

unsafe impl<T> Sync for SpinLock<T> {}
unsafe impl<T> Send for SpinLock<T> {}

impl<'a, T> Drop for SpinLockGuard<'a, T> {
    fn drop(&mut self) {
        self.spin_lock.lock.store(false, Ordering::Release);
    }
}

impl<'a, T> Deref for SpinLockGuard<'a, T> {
    type Target = T;

    fn deref(&self) -> &Self::Target {
        unsafe { &*self.spin_lock.data.get() }
    }
}

impl<'a, T> DerefMut for SpinLockGuard<'a, T> {
    fn deref_mut(&mut self) -> &mut Self::Target {
        unsafe { &mut *self.spin_lock.data.get() }
    }
}

fn main() {
    let lock = Arc::new(SpinLock::new(0));

    let mut handlers = vec![];

    for _ in 0..NUMBER_OF_THREADS {
        let lock_0 = lock.clone();

        let handler = thread::spawn(move || {
            for _ in 0..NUMBER_OF_LOOPS {
                let mut data = lock_0.lock();

                *data += 1;
            }
        });

        handlers.push(handler);
    }

    for handler in handlers {
        handler.join().unwrap();
    }

    println!(
        "actual count = {} (expected count = {})",
        *lock.lock(),
        NUMBER_OF_LOOPS * NUMBER_OF_THREADS
    );
}
