use std::sync::{Condvar, Mutex};

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

/*
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
    }
}
*/
