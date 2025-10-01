use std::cell::UnsafeCell;
use std::marker::{Send, Sync};
use std::mem::MaybeUninit;
use std::ops::Drop;
use std::sync::atomic::{AtomicBool, Ordering};
use std::thread;

pub struct Channel<T> {
    message: UnsafeCell<MaybeUninit<T>>,
    in_use: AtomicBool,
    ready: AtomicBool,
}

unsafe impl<T> Sync for Channel<T> where T: Send {}

impl<T> Drop for Channel<T> {
    fn drop(&mut self) {
        if *self.ready.get_mut() {
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
            in_use: AtomicBool::new(false),
            ready: AtomicBool::new(false),
        }
    }

    pub fn is_ready(&self) -> bool {
        self.ready.load(Ordering::Relaxed)
    }

    pub fn send(&self, message: T) {
        if self.in_use.swap(true, Ordering::Relaxed) {
            panic!("Cannot send more than one message !");
        }

        unsafe {
            (*self.message.get()).write(message);
        }

        self.ready.store(true, Ordering::Release);
    }

    /*
    pub fn receive(&self) -> T {
        if !self.ready.load(Ordering::Acquire) {
            panic!("No message available !");
        }

        unsafe { (*self.message.get()).assume_init_read() }
    }
    */

    pub fn receive(&self) -> T {
        if !self.ready.swap(false, Ordering::Acquire) {
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
