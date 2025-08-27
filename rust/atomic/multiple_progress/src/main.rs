use std::sync::atomic::AtomicUsize;
use std::sync::atomic::Ordering;
use std::thread;
use std::thread::sleep;
use std::time::Duration;

fn main() {
    let number_of_done = &AtomicUsize::new(0);

    thread::scope(|s| {
        for n in 0..4 {
            s.spawn(move || {
                for t in 0..25 {
                    sleep(Duration::from_secs(n + t));

                    number_of_done.fetch_add(1, Ordering::Relaxed);
                }
            });
        }

        loop {
            let n = number_of_done.load(Ordering::Relaxed);

            if n == 100 {
                break;
            }

            println!("Progress ... {n} / 100 done");

            sleep(Duration::from_secs(1));
        }
    });

    println!("Done !");
}
