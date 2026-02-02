use atomic_wait::{wait, wake_all, wake_one};
use std::cell::UnsafeCell;
use std::marker::{Send, Sync};
use std::ops::{Deref, DerefMut, Drop};
use std::sync::atomic::{AtomicU32, Ordering};

struct RWLock<T> {
    state: AtomicU32,
    writer_wake_counter: AtomicU32,
    value: UnsafeCell<T>,
}

unsafe impl<T> Sync for RWLock<T> where T: Send + Sync {}

impl<T> RWLock<T> {
    pub const fn new(value: T) -> Self {
        Self {
            state: AtomicU32::new(0),
            writer_wake_counter: AtomicU32::new(0),
            value: UnsafeCell::new(value),
        }
    }

    pub fn read(&self) -> ReadGuard<'_, T> {
        let mut s = self.state.load(Ordering::Relaxed);

        loop {
            if s < u32::MAX {
                assert!(s < u32::MAX - 1, "too many readers");

                match self.state.compare_exchange_weak(
                    s,
                    s + 1,
                    Ordering::Acquire,
                    Ordering::Relaxed,
                ) {
                    Ok(_) => return ReadGuard { rwlock: self },
                    Err(e) => s = e,
                }
            }

            if s == u32::MAX {
                wait(&self.state, u32::MAX);

                s = self.state.load(Ordering::Relaxed);
            }
        }
    }

    pub fn write(&self) -> WriteGuard<'_, T> {
        while self
            .state
            .compare_exchange(0, u32::MAX, Ordering::Acquire, Ordering::Relaxed)
            .is_err()
        {
            let w = self.state.load(Ordering::Acquire);

            if self.state.load(Ordering::Relaxed) != 0 {
                wait(&self.writer_wake_counter, w);
            }
        }

        WriteGuard { rwlock: self }
    }
}

struct ReadGuard<'a, T> {
    rwlock: &'a RWLock<T>,
}

impl<T> Deref for ReadGuard<'_, T> {
    type Target = T;

    fn deref(&self) -> &T {
        unsafe { &*self.rwlock.value.get() }
    }
}

impl<T> Drop for ReadGuard<'_, T> {
    fn drop(&mut self) {
        if self.rwlock.state.fetch_sub(1, Ordering::Release) == 1 {
            self.rwlock
                .writer_wake_counter
                .fetch_add(1, Ordering::Release);
            wake_one(&self.rwlock.writer_wake_counter);
        }
    }
}

struct WriteGuard<'a, T> {
    rwlock: &'a RWLock<T>,
}

impl<T> Deref for WriteGuard<'_, T> {
    type Target = T;

    fn deref(&self) -> &T {
        unsafe { &*self.rwlock.value.get() }
    }
}

impl<T> DerefMut for WriteGuard<'_, T> {
    fn deref_mut(&mut self) -> &mut T {
        unsafe { &mut *self.rwlock.value.get() }
    }
}

impl<T> Drop for WriteGuard<'_, T> {
    fn drop(&mut self) {
        self.rwlock.state.store(0, Ordering::Release);
        self.rwlock.writer_wake_counter.store(0, Ordering::Release);
        wake_one(&self.rwlock.writer_wake_counter);
        wake_all(&self.rwlock.state);
    }
}

fn main() {
    println!("Hello, world!");
}
