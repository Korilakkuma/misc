use std::cell::UnsafeCell;
use std::marker::{Send, Sync};
use std::ops::{Deref, DerefMut};
use std::sync::atomic::{fence, AtomicBool, AtomicUsize, Ordering};

pub const NUMBER_OF_LOCKS: usize = 8;

const MASK: usize = NUMBER_OF_LOCKS - 1;

pub struct FairLock<T> {
    waiting: Vec<AtomicBool>,
    lock: AtomicBool,
    turn: AtomicUsize,
    data: UnsafeCell<T>,
}

pub struct FairLockGuard<'a, T> {
    fair_lock: &'a FairLock<T>,
    index: usize,
}

impl<T> FairLock<T> {
    pub fn new(v: T) -> Self {
        let mut waitings = vec![];

        for _ in 0..NUMBER_OF_LOCKS {
            waitings.push(AtomicBool::new(false));
        }

        FairLock {
            waiting: waitings,
            lock: AtomicBool::new(false),
            turn: AtomicUsize::new(0),
            data: UnsafeCell::new(v),
        }
    }

    pub fn lock(&self, index: usize) -> FairLockGuard<T> {
        assert!(index < NUMBER_OF_LOCKS);

        self.waiting[index].store(true, Ordering::Relaxed);

        loop {
            if !self.waiting[index].load(Ordering::Relaxed) {
                break;
            }

            if !self.lock.load(Ordering::Relaxed) {
                if let Ok(_) = self.lock.compare_exchange_weak(
                    false,
                    true,
                    Ordering::Relaxed,
                    Ordering::Relaxed,
                ) {
                    break;
                }
            }
        }

        fence(Ordering::Acquire);

        FairLockGuard {
            fair_lock: self,
            index,
        }
    }
}

impl<'a, T> Drop for FairLockGuard<'a, T> {
    fn drop(&mut self) {
        let fl = self.fair_lock;

        fl.waiting[self.index].store(false, Ordering::Relaxed);

        let turn = fl.turn.load(Ordering::Relaxed);
        let next = if turn == self.index {
            (turn + 1) & MASK
        } else {
            turn
        };

        if fl.waiting[next].load(Ordering::Relaxed) {
            fl.turn.store(next, Ordering::Relaxed);
            fl.waiting[next].store(false, Ordering::Release);
        } else {
            fl.turn.store((next + 1) & MASK, Ordering::Relaxed);
            fl.lock.store(false, Ordering::Release);
        }
    }
}

unsafe impl<T> Send for FairLock<T> {}
unsafe impl<T> Sync for FairLock<T> {}

impl<'a, T> Deref for FairLockGuard<'a, T> {
    type Target = T;

    fn deref(&self) -> &Self::Target {
        unsafe { &*self.fair_lock.data.get() }
    }
}

impl<'a, T> DerefMut for FairLockGuard<'a, T> {
    fn deref_mut(&mut self) -> &mut Self::Target {
        unsafe { &mut *self.fair_lock.data.get() }
    }
}
