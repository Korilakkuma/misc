use std::boxed::Box;
use std::clone::Clone;
use std::marker::{Send, Sync};
use std::mem::drop;
use std::ops::{Deref, Drop};
use std::ptr::NonNull;
use std::sync::atomic::{fence, AtomicUsize, Ordering};

struct ArcData<T> {
    ref_count: AtomicUsize,
    data: T,
}

pub struct Arc<T> {
    ptr: NonNull<ArcData<T>>,
}

unsafe impl<T: Send + Sync> Send for Arc<T> {}
unsafe impl<T: Send + Sync> Sync for Arc<T> {}

impl<T> Deref for Arc<T> {
    type Target = T;

    fn deref(&self) -> &T {
        &self.data().data
    }
}

impl<T> Drop for Arc<T> {
    fn drop(&mut self) {
        if self.data().ref_count.fetch_sub(1, Ordering::Release) == 1 {
            fence(Ordering::Acquire);

            unsafe { drop(Box::from_raw(self.ptr.as_ptr())) }
        }
    }
}

impl<T> Clone for Arc<T> {
    fn clone(&self) -> Self {
        if self.data().ref_count.fetch_add(1, Ordering::Relaxed) > usize::MAX / 2 {
            std::process::abort();
        }

        Arc { ptr: self.ptr }
    }
}

impl<T> Arc<T> {
    pub fn new(data: T) -> Arc<T> {
        Arc {
            ptr: NonNull::from(Box::leak(Box::new(ArcData {
                ref_count: AtomicUsize::new(1),
                data,
            }))),
        }
    }

    pub fn get_mut(arc: &mut Self) -> Option<&mut T> {
        if arc.data().ref_count.load(Ordering::Relaxed) == 1 {
            fence(Ordering::Acquire);

            unsafe { Some(&mut arc.ptr.as_mut().data) }
        } else {
            None
        }
    }

    fn data(&self) -> &ArcData<T> {
        unsafe { self.ptr.as_ref() }
    }
}

fn main() {
    println!("Hello, world!");
}

#[test]
fn test() {
    static NUMBER_OF_DROPS: AtomicUsize = AtomicUsize::new(0);

    struct DropDetector;

    impl Drop for DropDetector {
        fn drop(&mut self) {
            NUMBER_OF_DROPS.fetch_add(1, Ordering::Relaxed);
        }
    }

    let x = Arc::new(("Hello", DropDetector));
    let y = x.clone();

    let t = std::thread::spawn(move || {
        assert_eq!(x.0, "Hello");
    });

    assert_eq!(y.0, "Hello");

    t.join().unwrap();

    assert_eq!(NUMBER_OF_DROPS.load(Ordering::Relaxed), 0);

    drop(y);

    assert_eq!(NUMBER_OF_DROPS.load(Ordering::Relaxed), 1);
}
