use std::thread;

fn main() {
    let l: &'static [i32; 3] = Box::leak(Box::new([1, 2, 3]));

    thread::spawn(move || dbg!(l));
    thread::spawn(move || dbg!(l));
}
