extern crate multi_thread;

use multi_thread::ThreadPool;

use std::io::prelude::*;
use std::net::TcpListener;
use std::net::TcpStream;
use std::fs::File;
use std::thread;
use std::time::Duration;

fn main() {
    let listener = TcpListener::bind("127.0.0.1:8888").unwrap();
    let pool = ThreadPool::new(4);

    for stream in listener.incoming().take(2) {
        pool.execute(|| {
            handle_connection(stream.unwrap());
        });
    }
}

fn handle_connection(mut stream: TcpStream) {
    let mut buffer = [0; 1024];

    stream.read(&mut buffer).unwrap();

    let index = b"GET / HTTP/1.1\r\n";
    let sleep = b"GET /sleep HTTP/1.1\r\n";

    if buffer.starts_with(index) {
        let mut file = File::open("index.html").unwrap();

        let mut contents = String::new();

        file.read_to_string(&mut contents).unwrap();

        let response = format!("HTTP/1.1 200 OK\r\n\r\n{}", contents);

        stream.write(response.as_bytes()).unwrap();
        stream.flush().unwrap();
    } else if buffer.starts_with(sleep) {
        thread::sleep(Duration::from_secs(5));

        let mut file = File::open("sleep.html").unwrap();

        let mut contents = String::new();

        file.read_to_string(&mut contents).unwrap();

        let response = format!("HTTP/1.1 200 OK\r\n\r\n{}", contents);

        stream.write(response.as_bytes()).unwrap();
        stream.flush().unwrap();
    } else {
        let status_line = "HTTP/1.1 404 NOT FOUND\r\n\r\n";

        let mut file = File::open("404.html").unwrap();

        let mut contents = String::new();

        file.read_to_string(&mut contents).unwrap();

        let response = format!("{}{}", status_line, contents);

        stream.write(response.as_bytes()).unwrap();
        stream.flush().unwrap();
    }
}
