use std::sync::mpsc::{channel, Sender};

fn main() {
    let mut handlers = vec![];

    let (tx, rx) = channel::<Sender<()>>();

    let barrier = move || {
        let x = rx.recv().unwrap();
        let y = rx.recv().unwrap();
        let z = rx.recv().unwrap();

        println!("Send !");

        x.send(()).unwrap();
        y.send(()).unwrap();
        z.send(()).unwrap();
    };

    let handler = std::thread::spawn(barrier);

    handlers.push(handler);

    for _ in 0..3 {
        let tx_c = tx.clone();

        let node = move || {
            let (tx0, rx0) = channel();

            tx_c.send(tx0).unwrap();

            rx0.recv().unwrap();

            println!("Received !");
        };

        let handler = std::thread::spawn(node);

        handlers.push(handler);
    }

    for handler in handlers {
        handler.join().unwrap();
    }
}
