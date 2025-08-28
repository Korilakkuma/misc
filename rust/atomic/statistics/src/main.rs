use std::sync::atomic::{AtomicU64, AtomicUsize, Ordering};
use std::thread;
use std::thread::sleep;
use std::time::{Duration, Instant};

fn main() {
    let number_of_done = &AtomicUsize::new(0);
    let total_time = &AtomicU64::new(0);
    let max_time = &AtomicU64::new(0);

    thread::scope(|s| {
        for t in 0..4 {
            s.spawn(move || {
                for i in 0..25 {
                    let start = Instant::now();

                    sleep(Duration::from_secs(t * i));

                    let time_taken = start.elapsed().as_micros() as u64;

                    number_of_done.fetch_add(1, Ordering::Relaxed);
                    total_time.fetch_add(time_taken, Ordering::Relaxed);
                    max_time.fetch_max(time_taken, Ordering::Relaxed);
                }
            });
        }

        loop {
            let total_time = Duration::from_micros(total_time.load(Ordering::Relaxed));
            let max_time = Duration::from_micros(max_time.load(Ordering::Relaxed));

            let n = number_of_done.load(Ordering::Relaxed);

            if n == 100 {
                break;
            }

            if n == 0 {
                println!("Progress ... nothing done yet.");
            } else {
                println!(
                    "Progress ... {n}/100 done, {:?} average, {:?} peak",
                    total_time / n as u32,
                    max_time,
                );
            }

            sleep(Duration::from_secs(1));
        }
    });

    println!("Done !");
}
