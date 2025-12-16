use std::boxed::Box;
use std::cell::UnsafeCell;

struct Mutex {
    m: Box<UnsafeCell<libc::pthread_mutex_t>>,
}

impl Mutex {
    pub fn new(data: libc::pthread_mutex_t) -> Mutex {
        Mutex {
            m: Box::new(UnsafeCell::new(data)),
        }
    }

    pub fn lock(&self) -> i32 {
        unsafe { libc::pthread_mutex_lock(self.m.get()) }
    }

    pub fn unlock(&self) -> i32 {
        unsafe { libc::pthread_mutex_unlock(self.m.get()) }
    }

    pub fn destroy(&self) {
        unsafe {
            libc::pthread_mutex_destroy(self.m.get());
        }
    }
}

fn main() {
    let p = libc::PTHREAD_MUTEX_INITIALIZER;
    let m = Mutex::new(p);

    let guard = m.lock();

    while guard != 0 {
        // do something ...
    }

    // std::mem::forget(guard);

    // ...

    m.unlock();
    m.destroy();
}
