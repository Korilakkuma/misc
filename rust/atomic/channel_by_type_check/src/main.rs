use std::cell::UnsafeCell;
use std::marker::{Send, Sync};
use std::mem::MaybeUninit;
use std::ops::Drop;
use std::sync::atomic::AtomicBool;
use std::sync::atomic::Ordering;
use std::sync::Arc;
use std::thread;

struct Channel<T> {
    message: UnsafeCell<MaybeUninit<T>>,
    ready: AtomicBool,
}

pub struct Sender<T> {
    channel: Arc<Channel<T>>,
}

pub struct Receiver<T> {
    channel: Arc<Channel<T>>,
}

unsafe impl<T> Send for Sender<T> {}
unsafe impl<T> Sync for Receiver<T> {}

pub fn channel<T>() -> (Sender<T>, Receiver<T>) {
    let a = Arc::new(Channel {
        message: UnsafeCell::new(MaybeUninit::uninit()),
        ready: AtomicBool::new(false),
    });

    (Sender { channel: a.clone() }, Receiver { channel: a })
}

impl<T> Sender<T> {
    pub fn send(self, message: T) {
        unsafe { (*self.channel.message.get()).write(message) };

        self.channel.ready.store(true, Ordering::Release);
    }
}

impl<T> Receiver<T> {
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

impl<T> Drop for Channel<T> {
    fn drop(&mut self) {
        if *self.ready.get_mut() {
            unsafe {
                (*self.message.get_mut()).assume_init_drop();
            }
        }
    }
}

fn main() {
    thread::scope(|s| {
        let (sender, receiver) = channel();

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
