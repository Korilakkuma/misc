use std::sync::Mutex;
use std::thread;

fn main() {
    let m = Mutex::new(0);

    thread::scope(|s| {
        for _ in 0..10 {
            s.spawn(|| {
                let mut guard = m.lock().unwrap();

                for _ in 0..100 {
                    *guard += 1;
                }
            });
        }
    });

    assert_eq!(m.into_inner().unwrap(), 1000);
}
