use std::boxed::Box;
use std::cell::UnsafeCell;
use std::clone::Clone;
use std::marker::{Send, Sync};
use std::ops::{Deref, Drop};
use std::ptr::NonNull;
use std::sync::atomic::{fence, AtomicUsize, Ordering};

struct ArcData<T> {
    data_ref_count: AtomicUsize,
    alloc_ref_count: AtomicUsize,
    data: UnsafeCell<Option<T>>,
}

pub struct Weak<T> {
    ptr: NonNull<ArcData<T>>,
}

pub struct Arc<T> {
    weak: Weak<T>,
}

unsafe impl<T: Send + Sync> Send for Weak<T> {}
unsafe impl<T: Send + Sync> Sync for Weak<T> {}

impl<T> Arc<T> {
    pub fn new(data: T) -> Arc<T> {
        Arc {
            weak: Weak {
                ptr: NonNull::from(Box::leak(Box::new(ArcData {
                    data_ref_count: AtomicUsize::new(1),
                    alloc_ref_count: AtomicUsize::new(1),
                    data: UnsafeCell::new(Some(data)),
                }))),
            },
        }
    }

    pub fn downgrade(arc: &Self) -> Weak<T> {
        arc.weak.clone()
    }

    pub fn get_mut(arc: &mut Self) -> Option<&mut T> {
        if arc.weak.data().alloc_ref_count.load(Ordering::Relaxed) == 1 {
            fence(Ordering::Acquire);

            let arcdata = unsafe { arc.weak.ptr.as_mut() };
            let option = arcdata.data.get_mut();

            let data = option.as_mut().unwrap();

            Some(data)
        } else {
            None
        }
    }
}

impl<T> Weak<T> {
    pub fn upgrade(&self) -> Option<Arc<T>> {
        let mut n = self.data().data_ref_count.load(Ordering::Relaxed);

        loop {
            if n == 0 {
                return None;
            }

            assert!(n < usize::MAX);

            if let Err(e) = self.data().data_ref_count.compare_exchange_weak(
                n,
                n + 1,
                Ordering::Relaxed,
                Ordering::Relaxed,
            ) {
                n = e;
                continue;
            }

            return Some(Arc { weak: self.clone() });
        }
    }

    fn data(&self) -> &ArcData<T> {
        unsafe { self.ptr.as_ref() }
    }
}

impl<T> Deref for Arc<T> {
    type Target = T;

    fn deref(&self) -> &T {
        let ptr = self.weak.data().data.get();

        unsafe { (*ptr).as_ref().unwrap() }
    }
}

impl<T> Clone for Weak<T> {
    fn clone(&self) -> Self {
        if self.data().alloc_ref_count.fetch_add(1, Ordering::Relaxed) > usize::MAX / 2 {
            std::process::abort();
        }

        Weak { ptr: self.ptr }
    }
}

impl<T> Clone for Arc<T> {
    fn clone(&self) -> Self {
        let weak = self.weak.clone();

        if self
            .weak
            .data()
            .alloc_ref_count
            .fetch_add(1, Ordering::Relaxed)
            > usize::MAX / 2
        {
            std::process::abort();
        }

        Arc { weak }
    }
}

impl<T> Drop for Weak<T> {
    fn drop(&mut self) {
        if self.data().alloc_ref_count.fetch_sub(1, Ordering::Release) == 1 {
            fence(Ordering::Acquire);

            unsafe { drop(Box::from_raw(self.ptr.as_ptr())) };
        }
    }
}

impl<T> Drop for Arc<T> {
    fn drop(&mut self) {
        if self
            .weak
            .data()
            .data_ref_count
            .fetch_sub(1, Ordering::Release)
            == 1
        {
            fence(Ordering::Acquire);

            let ptr = self.weak.data().data.get();

            unsafe {
                (*ptr) = None;
            }
        }
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
    let y = Arc::downgrade(&x);
    let z = Arc::downgrade(&x);

    let t = std::thread::spawn(move || {
        let y = y.upgrade().unwrap();

        assert_eq!(y.0, "Hello");
    });

    assert_eq!(x.0, "Hello");

    t.join().unwrap();

    assert_eq!(NUMBER_OF_DROPS.load(Ordering::Relaxed), 0);
    assert!(z.upgrade().is_some());

    drop(x);

    assert_eq!(NUMBER_OF_DROPS.load(Ordering::Relaxed), 1);
    assert!(z.upgrade().is_none());
}
