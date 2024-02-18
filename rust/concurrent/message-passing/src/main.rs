use std::thread;
use std::sync::mpsc;

fn main() {
    let (transfer, receiver) = mpsc::channel();

    thread::spawn(move || {
        let val = String::from("from transfer");

        transfer.send(val).unwrap();

        let vals = vec![String::from("from"), String::from("transfer"), String::from ("!")];

        for v in vals {
            transfer.send(v).unwrap();
        }

        // println!("value is {}", val);
    });

    for r in receiver {
      println!("Received: {}", r);
    }
}
