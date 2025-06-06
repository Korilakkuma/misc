use std::time;
use tokio::sync::oneshot;

async fn set_val_later(tx: oneshot::Sender<i32>) {
    let sec = time::Duration::from_secs(10);

    println!("Asynchronous sleep ...");

    tokio::time::sleep(sec).await;

    if let Err(_) = tx.send(100) {
        println!("Failed to send");
    }
}

#[tokio::main]
pub async fn main() {
    let (tx, rx) = oneshot::channel();

    tokio::spawn(set_val_later(tx));

    match rx.await {
        Ok(n) => {
            println!("n = {}", n);
        }
        Err(e) => {
            println!("Failed to receive: {}", e);
            return;
        }
    }
}
