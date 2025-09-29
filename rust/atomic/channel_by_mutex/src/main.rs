use std::collections::VecDeque;
use std::sync::{Condvar, Mutex};
use std::thread;

pub struct Channel<T> {
    queue: Mutex<VecDeque<T>>,
    item_ready: Condvar,
}

impl<T> Channel<T> {
    pub fn new() -> Self {
        Self {
            queue: Mutex::new(VecDeque::new()),
            item_ready: Condvar::new(),
        }
    }

    pub fn send(&self, message: T) {
        self.queue.lock().unwrap().push_back(message);
        self.item_ready.notify_one();
    }

    pub fn receive(&self) -> T {
        let mut q = self.queue.lock().unwrap();

        loop {
            if let Some(message) = q.pop_front() {
                return message;
            }

            q = self.item_ready.wait(q).unwrap();
        }
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
