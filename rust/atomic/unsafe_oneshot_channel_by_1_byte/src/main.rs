use std::cell::UnsafeCell;
use std::marker::{Send, Sync};
use std::mem::MaybeUninit;
use std::ops::Drop;
use std::sync::atomic::AtomicU8;
use std::sync::atomic::Ordering;
use std::thread;

const EMPTY: u8 = 0;
const WAITING: u8 = 1;
const READY: u8 = 2;
const READING: u8 = 3;

pub struct Channel<T> {
    message: UnsafeCell<MaybeUninit<T>>,
    state: AtomicU8,
}

unsafe impl<T: Send> Sync for Channel<T> {}

impl<T> Drop for Channel<T> {
    fn drop(&mut self) {
        if *self.state.get_mut() == READY {
            unsafe {
                self.message.get_mut().assume_init_drop();
            }
        }
    }
}

impl<T> Channel<T> {
    pub const fn new() -> Self {
        Self {
            message: UnsafeCell::new(MaybeUninit::uninit()),
            state: AtomicU8::new(EMPTY),
        }
    }

    pub fn is_ready(&self) -> bool {
        self.state.load(Ordering::Relaxed) == READY
    }

    pub fn send(&self, message: T) {
        if self
            .state
            .compare_exchange(EMPTY, WAITING, Ordering::Relaxed, Ordering::Relaxed)
            .is_err()
        {
            panic!("Cannot send more than one message !");
        }

        unsafe { (*self.message.get()).write(message) };

        self.state.store(READY, Ordering::Release);
    }

    pub fn receive(&self) -> T {
        if self
            .state
            .compare_exchange(READY, READING, Ordering::Acquire, Ordering::Relaxed)
            .is_err()
        {
            panic!("No message available !");
        }

        unsafe { (*self.message.get()).assume_init_read() }
    }
}

fn main() {
    let channel = Channel::new();

    let t = thread::current();

    thread::scope(|s| {
        s.spawn(|| {
            channel.send("Hello World !");
            t.unpark();
        });

        while !channel.is_ready() {
            thread::park();
        }

        assert_eq!(channel.receive(), "Hello World !");
    });
}
