use std::thread;

// static S: [i32; 3] = [1, 2, 3];

fn main() {
    static S: [i32; 3] = [1, 2, 3];

    thread::spawn(|| dbg!(&S));
    thread::spawn(|| dbg!(&S));
}
