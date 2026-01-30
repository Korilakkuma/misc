use atomic_wait::{wait, wake_all, wake_one};
use std::cell::UnsafeCell;
use std::marker::{Send, Sync};
use std::mem::drop;
use std::ops::{Deref, DerefMut, Drop};
use std::sync::atomic::{AtomicU32, AtomicUsize, Ordering};
use std::thread;
use std::time::Instant;

struct Mutex<T> {
    state: AtomicU32,
    value: UnsafeCell<T>,
}

struct MutexGuard<'a, T> {
    mutex: &'a Mutex<T>,
}

unsafe impl<T> Send for MutexGuard<'_, T> where T: Send {}
unsafe impl<T> Sync for MutexGuard<'_, T> where T: Sync {}

impl<T> Deref for MutexGuard<'_, T> {
    type Target = T;

    fn deref(&self) -> &T {
        unsafe { &*self.mutex.value.get() }
    }
}

impl<T> DerefMut for MutexGuard<'_, T> {
    fn deref_mut(&mut self) -> &mut T {
        unsafe { &mut *self.mutex.value.get() }
    }
}

impl<T> Drop for MutexGuard<'_, T> {
    fn drop(&mut self) {
        if self.mutex.state.swap(0, Ordering::Release) == 2 {
            wake_one(&self.mutex.state);
        }
    }
}

unsafe impl<T> Send for Mutex<T> where T: Send {}
unsafe impl<T> Sync for Mutex<T> where T: Sync {}

impl<T> Mutex<T> {
    pub const fn new(value: T) -> Self {
        Self {
            state: AtomicU32::new(0),
            value: UnsafeCell::new(value),
        }
    }

    pub fn lock(&self) -> MutexGuard<'_, T> {
        if self
            .state
            .compare_exchange(0, 1, Ordering::Acquire, Ordering::Relaxed)
            .is_err()
        {
            lock_contended(&self.state);
        }

        MutexGuard { mutex: self }
    }
}

fn lock_contended(state: &AtomicU32) {
    let mut spin_count = 0;

    while state.load(Ordering::Relaxed) == 1 && spin_count < 100 {
        spin_count += 1;
        std::hint::spin_loop();
    }

    if state
        .compare_exchange(0, 1, Ordering::Acquire, Ordering::Relaxed)
        .is_ok()
    {
        return;
    }

    while state.swap(2, Ordering::Acquire) != 0 {
        wait(state, 2);
    }
}
struct Condvar {
    counter: AtomicU32,
    number_of_waiters: AtomicUsize,
}

impl Condvar {
    pub const fn new() -> Self {
        Self {
            counter: AtomicU32::new(0),
            number_of_waiters: AtomicUsize::new(0),
        }
    }

    pub fn notify_one(&self) {
        if self.number_of_waiters.load(Ordering::Relaxed) > 0 {
            self.counter.fetch_add(1, Ordering::Relaxed);
            wake_one(&self.counter);
        }
    }

    pub fn notify_all(&self) {
        if self.number_of_waiters.load(Ordering::Relaxed) > 0 {
            self.counter.fetch_add(1, Ordering::Relaxed);
            wake_all(&self.counter);
        }
    }

    pub fn wait<'a, T>(&self, guard: MutexGuard<'a, T>) -> MutexGuard<'a, T> {
        self.number_of_waiters.fetch_add(1, Ordering::Relaxed);

        let counter_value = self.counter.load(Ordering::Relaxed);

        let mutex = guard.mutex;

        drop(guard);

        wait(&self.counter, counter_value);

        self.number_of_waiters.fetch_sub(1, Ordering::Relaxed);

        mutex.lock()
    }
}

fn main() {
    let m = Mutex::new(0);

    std::hint::black_box(&m);

    let start = Instant::now();

    /*
    for _ in 0..5_000_000 {
        *m.lock() += 1;
    }
    */

    thread::scope(|s| {
        for _ in 0..4 {
            s.spawn(|| {
                for _ in 0..5_000_000 {
                    *m.lock() += 1;
                }
            });
        }
    });

    let duration = start.elapsed();

    println!("locked {} times in {:?}", *m.lock(), duration);
}

#[test]
fn test_condvar() {
    let mutex = Mutex::new(0);
    let condvar = Condvar::new();

    let mut wakeups = 0;

    std::thread::scope(|s| {
        s.spawn(|| {
            std::thread::sleep(std::time::Duration::from_secs(1));

            *mutex.lock() = 527;

            condvar.notify_one();
        });

        let mut m = mutex.lock();

        while *m < 100 {
            m = condvar.wait(m);

            wakeups += 1;
        }

        assert_eq!(*m, 527);
    });

    assert!(wakeups < 10);
}
