use std::collections::VecDeque;
use std::sync::Mutex;
use std::thread;
use std::time::Duration;

fn main() {
    let queue = Mutex::new(VecDeque::new());

    thread::scope(|s| {
        let consumer = s.spawn(|| loop {
            let item = queue.lock().unwrap().pop_front();

            if let Some(item) = item {
                dbg!(item);
            } else {
                thread::park();
            }
        });

        for n in 0.. {
            queue.lock().unwrap().push_back(n);
            consumer.thread().unpark();
            thread::sleep(Duration::from_secs(1));
        }
    });
}
