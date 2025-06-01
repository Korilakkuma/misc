use tokio::io;
use tokio::io::{AsyncBufReadExt, AsyncWriteExt};
use tokio::net::TcpListener;

#[tokio::main]
async fn main() -> io::Result<()> {
    let listener = TcpListener::bind("127.0.0.1:3000").await.unwrap();

    loop {
        let (mut socket, addr) = listener.accept().await?;

        println!("Accept: {}", addr);

        tokio::spawn(async move {
            let (r, w) = socket.split();

            let mut reader = io::BufReader::new(r);
            let mut writer = io::BufWriter::new(w);

            let mut line = String::new();

            loop {
                line.clear();

                match reader.read_line(&mut line).await {
                    Ok(0) => {
                        println!("Closed: {}", addr);
                        return;
                    }
                    Ok(_) => {
                        print!("Read: {}, {}", addr, line);

                        writer.write_all(line.as_bytes()).await.unwrap();
                        writer.flush().await.unwrap();
                    }
                    Err(e) => {
                        println!("Error: {}, {}", addr, e);
                    }
                }
            }
        });
    }
}
