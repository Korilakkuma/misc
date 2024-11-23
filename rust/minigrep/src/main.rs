extern crate minigrep;

use std::env;
use std::process;

use minigrep::{ Config, run };

fn main() {
    let args: Vec<String> = env::args().collect();

    let config = Config::new(&args).unwrap_or_else(|error| {
        println!("{}", error);
        process::exit(1);
    });

    println!("Searching for {}", config.query);
    println!("in file {}", config.filename);

    if let Err(error) = run(config) {
        println!("Application error: {}", error);
        process::exit(1);
    }
}
