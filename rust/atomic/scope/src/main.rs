use std::thread;

fn main() {
    let numbers = Vec::from_iter(0..=1000);

    thread::scope(|s| {
        s.spawn(|| {
            println!("length is {}", numbers.len());
        });
        s.spawn(|| {
            for n in &numbers {
                println!("{n}");
            }
        });
    });
}
