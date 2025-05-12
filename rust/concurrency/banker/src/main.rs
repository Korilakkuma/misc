mod banker;

use banker::Banker;
use std::thread;

const NUMBER_OF_LOOPS: usize = 100000;

fn main() {
    let banker_0 = Banker::<2, 2>::new([1, 1], [[1, 1], [1, 1]]);
    let banker_1 = banker_0.clone();

    let thread_0 = thread::spawn(move || {
        for _ in 0..NUMBER_OF_LOOPS {
            while !banker_0.take(0, 0) {}
            while !banker_0.take(0, 1) {}

            println!("thread 0: Acquire Lock");

            banker_0.release(0, 0);
            banker_0.release(0, 1);

            println!("thread 0: Release Lock");
        }
    });

    let thread_1 = thread::spawn(move || {
        for _ in 0..NUMBER_OF_LOOPS {
            while !banker_1.take(1, 1) {}
            while !banker_1.take(1, 0) {}

            println!("thread 1: Acquire Lock");

            banker_1.release(1, 1);
            banker_1.release(1, 0);

            println!("thread 1: Release Lock");
        }
    });

    thread_0.join().unwrap();
    thread_1.join().unwrap();

    println!("Ended");
}
