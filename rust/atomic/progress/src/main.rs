use std::sync::atomic::AtomicUsize;
use std::sync::atomic::Ordering;
use std::thread;
use std::thread::sleep;
use std::time::Duration;

/*
fn main() {
    let number_of_done = AtomicUsize::new(0);

    thread::scope(|s| {
        s.spawn(|| {
            for n in 0..100 {
                sleep(Duration::from_secs(1));
                number_of_done.store(n + 1, Ordering::Relaxed);
            }
        });

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
*/

fn main() {
    let number_of_done = AtomicUsize::new(0);

    let main_thread = thread::current();

    thread::scope(|s| {
        s.spawn(|| {
            for n in 0..100 {
                sleep(Duration::from_secs(1));
                number_of_done.store(n + 1, Ordering::Relaxed);
                main_thread.unpark();
            }
        });

        loop {
            let n = number_of_done.load(Ordering::Relaxed);

            if n == 100 {
                break;
            }

            println!("Progress ... {n} / 100 done");

            thread::park_timeout(Duration::from_secs(1));
        }
    });

    println!("Done !");
}
