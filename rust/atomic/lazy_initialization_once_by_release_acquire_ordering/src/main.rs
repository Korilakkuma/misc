use std::mem::drop;
use std::ptr::null_mut;
use std::sync::atomic::{AtomicPtr, Ordering};

type Data = u64;

fn main() {
    let key = get_data();

    println!("Got key is {}", key);
}

fn get_data() -> &'static Data {
    static PTR: AtomicPtr<Data> = AtomicPtr::new(null_mut());

    let mut p = PTR.load(Ordering::Acquire);

    if p.is_null() {
        p = Box::into_raw(Box::new(generate_random_data()));

        if let Err(e) = PTR.compare_exchange(null_mut(), p, Ordering::Release, Ordering::Acquire) {
            drop(unsafe { Box::from_raw(p) });

            p = e;
        }
    }

    unsafe { &*p }
}

fn generate_random_data() -> Data {
    10
}
