use std::collections::LinkedList;
use std::sync::{Arc, Condvar, Mutex};

pub struct Semaphore {
    mutex: Mutex<isize>,
    condition: Condvar,
    max_size: isize,
}

impl Semaphore {
    pub fn new(max_size: isize) -> Self {
        Semaphore {
            mutex: Mutex::new(0),
            condition: Condvar::new(),
            max_size,
        }
    }

    pub fn wait(&self) {
        match self.mutex.lock() {
            Ok(mut count) => {
                while *count >= self.max_size {
                    match self.condition.wait(count) {
                        Ok(new_count) => count = new_count,
                        Err(_) => panic!("wait"),
                    }
                }
            }
            Err(error) => panic!("wait {}", error),
        }
    }

    pub fn post(&self) {
        match self.mutex.lock() {
            Ok(mut count) => {
                *count -= 1;

                if *count <= self.max_size {
                    self.condition.notify_one();
                }
            }
            Err(error) => panic!("post {}", error),
        }
    }
}

#[derive(Clone)]
pub struct Sender<T> {
    sem: Arc<Semaphore>,
    buf: Arc<Mutex<LinkedList<T>>>,
    cond: Arc<Condvar>,
}

impl<T: Send> Sender<T> {
    pub fn send(&self, data: T) {
        self.sem.wait();

        let mut buf = self.buf.lock().unwrap();

        buf.push_back(data);

        self.cond.notify_one();
    }
}

pub struct Receiver<T> {
    sem: Arc<Semaphore>,
    buf: Arc<Mutex<LinkedList<T>>>,
    cond: Arc<Condvar>,
}

impl<T> Receiver<T> {
    pub fn receive(&self) -> T {
        match self.buf.lock() {
            Ok(mut buf) => loop {
                if let Some(data) = buf.pop_front() {
                    self.sem.post();
                    return data;
                }

                buf = self.cond.wait(buf).unwrap();
            },
            Err(error) => panic!("{}", error),
        }
    }
}

pub fn channel<T>(max_size: isize) -> (Sender<T>, Receiver<T>) {
    assert!(max_size > 0);

    let sem = Arc::new(Semaphore::new(max_size));
    let buf = Arc::new(Mutex::new(LinkedList::new()));
    let cond = Arc::new(Condvar::new());

    let tx = Sender {
        sem: sem.clone(),
        buf: buf.clone(),
        cond: cond.clone(),
    };

    let rx = Receiver { sem, buf, cond };

    (tx, rx)
}
