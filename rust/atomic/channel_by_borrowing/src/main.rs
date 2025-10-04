use std::cell::UnsafeCell;
use std::marker::{Send, Sync};
use std::mem::MaybeUninit;
use std::ops::Drop;
use std::sync::atomic::{AtomicBool, Ordering};
use std::thread;

pub struct Channel<T> {
    message: UnsafeCell<MaybeUninit<T>>,
    ready: AtomicBool,
}

unsafe impl<T: Send + Sync> Sync for Channel<T> {}

pub struct Sender<'a, T> {
    channel: &'a Channel<T>,
}

pub struct Receiver<'a, T> {
    channel: &'a Channel<T>,
}

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
            ready: AtomicBool::new(false),
        }
    }

    pub fn split<'a>(&'a mut self) -> (Sender<'a, T>, Receiver<'a, T>) {
        *self = Self::new();

        (Sender { channel: self }, Receiver { channel: self })
    }
}

impl<T> Sender<'_, T> {
    pub fn send(self, message: T) {
        unsafe { (*self.channel.message.get()).write(message) };

        self.channel.ready.store(true, Ordering::Release);
    }
}

impl<T> Receiver<'_, T> {
    pub fn is_ready(&self) -> bool {
        self.channel.ready.load(Ordering::Relaxed)
    }

    pub fn receive(self) -> T {
        if !self.channel.ready.swap(false, Ordering::Acquire) {
            panic!("No message available !");
        }

        unsafe { (*self.channel.message.get()).assume_init_read() }
    }
}

fn main() {
    let mut channel = Channel::new();

    thread::scope(|s| {
        let (sender, receiver) = channel.split();

        let t = thread::current();

        s.spawn(move || {
            sender.send("Hello World !");
            t.unpark();
        });

        while !receiver.is_ready() {
            thread::park();
        }

        assert_eq!(receiver.receive(), "Hello World !");
    });
}
