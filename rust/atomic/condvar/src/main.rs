use std::collections::VecDeque;
use std::mem::drop;
use std::sync::Condvar;
use std::sync::Mutex;
use std::thread;
use std::time::Duration;

fn main() {
    let queue = Mutex::new(VecDeque::new());
    let cond = Condvar::new();

    thread::scope(|s| {
        s.spawn(|| loop {
            let mut q = queue.lock().unwrap();

            let item = loop {
                if let Some(item) = q.pop_front() {
                    break item;
                } else {
                    q = cond.wait(q).unwrap();
                }
            };

            drop(q);

            dbg!(item);
        });

        for n in 0.. {
            queue.lock().unwrap().push_back(n);
            cond.notify_one();
            thread::sleep(Duration::from_secs(1));
        }
    });
}
