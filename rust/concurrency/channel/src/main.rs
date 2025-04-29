use channel::channel;
use std::thread;

const NUMBER_OF_LOOPS: usize = 3;
const NUMBER_OF_THREADS: usize = 8;

fn main() {
    let (tx, rx) = channel(4);

    let mut handlers = vec![];

    let receiver = thread::spawn(move || {
        let mut count = 0;

        while count < (NUMBER_OF_THREADS * NUMBER_OF_LOOPS) {
            let n = rx.receive();

            println!("recv: n = {:?}", n);

            count += 1;
        }
    });

    handlers.push(receiver);

    for i in 0..NUMBER_OF_THREADS {
        let cloned_tx = tx.clone();

        let sender = thread::spawn(move || {
            for j in 0..NUMBER_OF_LOOPS {
                cloned_tx.send((i, j));
            }
        });

        handlers.push(sender);
    }

    for handler in handlers {
        handler.join().unwrap();
    }
}
