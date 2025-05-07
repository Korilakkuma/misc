use std::ptr::{read_volatile, write_volatile};
use std::sync::atomic::{fence, Ordering};
use std::thread;

const NUMBER_OF_LOOPS: usize = 3;
const NUMBER_OF_THREADS: usize = 8;

macro_rules! read_mem {
    ($address: expr) => {
        unsafe { read_volatile($address) }
    };
}

macro_rules! write_mem {
    ($address: expr, $value: expr) => {
        unsafe { write_volatile($address, $value) }
    };
}

static mut LOCK: BakeryLock = BakeryLock {
    entering: [false; NUMBER_OF_THREADS],
    tickets: [None; NUMBER_OF_THREADS],
};

static mut COUNT: u64 = 0;

struct BakeryLock {
    entering: [bool; NUMBER_OF_THREADS],
    tickets: [Option<u64>; NUMBER_OF_THREADS],
}

impl BakeryLock {
    fn lock(&mut self, index: usize) -> LockGuard {
        fence(Ordering::SeqCst);
        write_mem!(&mut self.entering[index], true);
        fence(Ordering::SeqCst);

        let mut max = 0;

        for i in 0..NUMBER_OF_THREADS {
            if let Some(t) = read_mem!(&self.tickets[i]) {
                max = max.max(t);
            }
        }

        let ticket = max + 1;

        write_mem!(&mut self.tickets[index], Some(ticket));

        fence(Ordering::SeqCst);
        write_mem!(&mut self.entering[index], false);
        fence(Ordering::SeqCst);

        for i in 0..NUMBER_OF_THREADS {
            if i == index {
                continue;
            }

            while read_mem!(&self.entering[i]) {}

            loop {
                match read_mem!(&self.tickets[i]) {
                    Some(t) => {
                        if ticket < t || (ticket == t && index < i) {
                            break;
                        }
                    }
                    None => {
                        break;
                    }
                }
            }
        }

        fence(Ordering::SeqCst);
        LockGuard { index }
    }
}

struct LockGuard {
    index: usize,
}

impl Drop for LockGuard {
    fn drop(&mut self) {
        fence(Ordering::SeqCst);
        write_mem!(&mut LOCK.tickets[self.index], None);
    }
}

fn main() {
    let mut threads = vec![];

    for index in 0..NUMBER_OF_LOOPS {
        let thread = thread::spawn(move || {
            for _ in 0..NUMBER_OF_LOOPS {
                let _lock = unsafe { (*(&raw mut LOCK)).lock(index) };

                unsafe {
                    let c = read_volatile(&raw const COUNT);
                    write_volatile(&raw mut COUNT, c + 1);
                }
            }
        });

        threads.push(thread);
    }

    for thread in threads {
        thread.join().unwrap();
    }

    println!(
        "COUNT = {} (Expected = {})",
        unsafe { COUNT },
        NUMBER_OF_LOOPS * NUMBER_OF_THREADS
    );
}
