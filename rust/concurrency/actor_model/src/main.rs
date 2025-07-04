mod green;

fn main() {
    green::spawn_from_main(producer, 2 * 1024 * 1024);
}

fn producer() {
    let id = green::spawn(consumer, 2 * 1024 * 1024);

    for msg in 0..10 {
        green::send(id, msg);
    }
}

fn consumer() {
    for _ in 0..10 {
        if let Some(msg) = green::recv() {
            println!("Received: count = {}", msg);
        }
    }
}
