use std::sync::Arc;
use std::time;
use tokio::sync::Mutex;

const NUMBER_OF_TASKS: usize = 9;

async fn lock_only(v: Arc<Mutex<u64>>) {
    let mut n = v.lock().await;

    println!("Lock only");

    *n += 1;
}

async fn lock_sleep(v: Arc<Mutex<u64>>) {
    let mut n = v.lock().await;

    let sec = time::Duration::from_secs(10);

    println!("Asynchronous sleep ...");

    tokio::time::sleep(sec).await;

    *n += 1;
}

#[tokio::main]
async fn main() -> Result<(), tokio::task::JoinError> {
    let val = Arc::new(Mutex::new(0));

    let mut handlers = vec![];

    let handler = tokio::spawn(lock_sleep(val.clone()));

    handlers.push(handler);

    for _ in 0..NUMBER_OF_TASKS {
        let n = val.clone();

        let handler = tokio::spawn(lock_only(n));

        handlers.push(handler);
    }

    for handler in handlers {
        handler.await?;
    }

    println!("Done !");

    Ok(())
}
