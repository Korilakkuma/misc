use std::time;

#[tokio::main]
async fn main() {
    tokio::join!(async move {
        let sec = time::Duration::from_secs(10);

        println!("Asynchronous sleep ...");

        tokio::time::sleep(sec).await;
    });

    println!("Done !");
}
