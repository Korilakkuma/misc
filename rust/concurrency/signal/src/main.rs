use libc::SIGINT;
use signal_hook::iterator::Signals;
use std::{error::Error, process, thread, time::Duration};

fn main() -> Result<(), Box<dyn Error>> {
    println!("process id: {}", process::id());

    let mut signals = Signals::new(&[SIGINT])?;

    let handler = thread::spawn(move || {
        for sig in signals.forever() {
            println!("Received signal: {:?}", sig)
        }
    });

    thread::sleep(Duration::from_secs(10));

    handler.join().unwrap();

    Ok(())
}
