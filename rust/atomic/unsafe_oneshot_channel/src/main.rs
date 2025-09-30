use std::cell::UnsafeCell;
use std::marker::{Send, Sync};
use std::mem::MaybeUninit;
use std::sync::atomic::{AtomicBool, Ordering};
use std::thread;

pub struct Channel<T> {
    message: UnsafeCell<MaybeUninit<T>>,
    ready: AtomicBool,
}

unsafe impl<T> Sync for Channel<T> where T: Send {}

impl<T> Channel<T> {
    pub const fn new() -> Self {
        Self {
            message: UnsafeCell::new(MaybeUninit::uninit()),
            ready: AtomicBool::new(false),
        }
    }

    pub fn is_ready(&self) -> bool {
        self.ready.load(Ordering::Acquire)
    }

    pub fn send(&self, message: T) {
        unsafe {
            (*self.message.get()).write(message);
        }
        self.ready.store(true, Ordering::Release);
    }

    pub fn receive(&self) -> T {
        unsafe { (*self.message.get()).assume_init_read() }
    }
}

fn main() {
    let channel = Channel::new();

    thread::scope(|s| {
        s.spawn(|| {
            channel.send("Hello World !");
        });
        s.spawn(|| {
            let m = channel.receive();

            println!("Receive: {}", m);
        });
    });
}
