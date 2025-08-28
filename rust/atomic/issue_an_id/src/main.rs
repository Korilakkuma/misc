use std::sync::atomic::AtomicU32;
use std::sync::atomic::Ordering;

fn main() {
    loop {
        let id = allocate_new_id();

        println!("ID is {}", id);
    }
}

fn allocate_new_id() -> u32 {
    static NEXT_ID: AtomicU32 = AtomicU32::new(0);

    let mut id = NEXT_ID.load(Ordering::Relaxed);

    loop {
        assert!(id < 1000, "Too many IDs !");

        match NEXT_ID.compare_exchange_weak(id, id + 1, Ordering::Relaxed, Ordering::Relaxed) {
            Ok(_) => return id,
            Err(v) => id = v,
        }

        // Or,
        // return NEXT_ID.fetch_update(Ordering::Relaxed, Ordering::Relaxed, |n| n.checked_add(1)).expect("Too many IDs !");
    }
}

/*
fn allocate_new_id() -> u32 {
    static NEXT_ID: AtomicU32 = AtomicU32::new(0);

    NEXT_ID.fetch_add(1, Ordering::Relaxed)
}
*/

// There is Problem
/*
fn allocate_new_id_overflow() -> u32 {
    static NEXT_ID: AtomicU32 = AtomicU32::new(0);

    let id = NEXT_ID.fetch_add(1, Ordering::Relaxed);

    assert!(id < 1000, "Too many IDs !");

    id
}
*/
