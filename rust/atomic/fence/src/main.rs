use std::sync::atomic::fence;
use std::sync::atomic::{AtomicBool, Ordering};
use std::thread;
use std::time::Duration;

static mut DATA: [usize; 10] = [0; 10];

const ATOMIC_FALSE: AtomicBool = AtomicBool::new(false);

static READY: [AtomicBool; 10] = [ATOMIC_FALSE; 10];

fn main() {
    for i in 0..10 {
        thread::spawn(move || {
            let data = thread_fun(i);

            unsafe { DATA[i] = data }

            READY[i].store(true, Ordering::Release);
        });
    }

    thread::sleep(Duration::from_millis(500));

    let ready: [bool; 10] = std::array::from_fn(|i| READY[i].load(Ordering::Relaxed));

    if ready.contains(&true) {
        fence(Ordering::Acquire);

        for i in 0..10 {
            if ready[i] {
                println!("data[{i}] = {}", unsafe { DATA[i] });
            }
        }
    }
}

fn thread_fun(data: usize) -> usize {
    data * 2
}
