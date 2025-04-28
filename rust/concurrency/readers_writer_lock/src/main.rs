use std::sync::RwLock;

fn main() {
    let rwlock = RwLock::new(10);

    {
        for id in 0..10 {
            match rwlock.read() {
                Ok(value) => {
                    println!("reader thread id[{}]: value = {}", id, value);
                }
                Err(error) => panic!("{}", error),
            }
        }
    }

    {
        match rwlock.write() {
            Ok(mut value) => {
                *value += 1;

                println!("writer thread]: value = {}", value);
            }
            Err(error) => panic!("{}", error),
        }
    }
}
