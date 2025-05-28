use std::io::{BufRead, BufReader, BufWriter, Write};
use std::net::TcpListener;

fn main() {
    match TcpListener::bind("127.0.0.1:3000") {
        Ok(listener) => {
            while let Ok((stream, _)) = listener.accept() {
                let stream_0 = stream.try_clone().unwrap();
                let stream_1 = stream.try_clone().unwrap();

                let mut reader = BufReader::new(stream_0);
                let mut writer = BufWriter::new(stream_1);

                let mut buf = String::new();

                reader.read_line(&mut buf).unwrap();
                writer.write(buf.as_bytes()).unwrap();
                writer.flush().unwrap();
            }
        }
        Err(error) => panic!("{}", error),
    }
}
