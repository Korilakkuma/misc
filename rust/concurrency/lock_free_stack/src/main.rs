// #![feature(asm)]

mod stack;

use stack::StackHead;
use std::sync::Arc;
use std::thread;

const NUMBER_OF_LOOPS: usize = 1000000;
const NUMBER_OF_THREADS: usize = 4;

fn main() {
    let stack = Arc::new(StackHead::<usize>::new());

    let mut handlers = vec![];

    for i in 0..NUMBER_OF_THREADS {
        let stack0 = stack.clone();

        let handler = thread::spawn(move || {
            if i & 1 == 0 {
                // even
                for j in 0..NUMBER_OF_LOOPS {
                    let k = j + i * NUMBER_OF_LOOPS;

                    stack0.push(k);

                    println!("End push: #{}", i);
                }
            } else {
                // odd
                for _ in 0..NUMBER_OF_LOOPS {
                    loop {
                        if let Some(k) = stack0.pop() {
                            println!("pop: {}", k);
                            break;
                        }
                    }
                }

                println!("End pop: #{}", i);
            }
        });

        handlers.push(handler);
    }

    for handler in handlers {
        handler.join().unwrap();
    }

    assert!(stack.pop() == None);
}
