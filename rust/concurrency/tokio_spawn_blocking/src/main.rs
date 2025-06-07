use std::time;

fn do_block(n: u64) -> u64 {
    let sec = time::Duration::from_secs(10);

    println!("Synchronous sleep ...");

    std::thread::sleep(sec);

    n
}

async fn do_print() {
    let sec = time::Duration::from_secs(1);

    for _ in 0..20 {
        println!("Asynchronous sleep ...");

        tokio::time::sleep(sec).await;

        println!("Wake up !");
    }
}

#[tokio::main]
pub async fn main() {
    let mut handlers = vec![];

    for n in 0..32 {
        let handler = tokio::task::spawn_blocking(move || do_block(n));

        handlers.push(handler);
    }

    let p = tokio::spawn(do_print());

    for handler in handlers {
        let n = handler.await.unwrap();

        println!("Done: {}", n);
    }

    p.await.unwrap();
}
