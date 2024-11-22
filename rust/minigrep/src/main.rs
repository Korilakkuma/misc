use std::env;
use std::fs::File;
use std::io::prelude::*;

fn main() {
    let args: Vec<String> = env::args().collect();

    let config = Config::new(&args);

    println!("Searching for {}", config.query);
    println!("in file {}", config.filename);

    let mut file = File::open(config.filename).expect("File is not found");

    let mut contents = String::new();

    file.read_to_string(&mut contents).expect("Error occurred in reading file");

    println!("File contents:\n{}", contents);
}

struct Config {
    query: String,
    filename: String,
}

impl Config {
    fn new(args: &[String]) -> Config {
        let query = args[1].clone();
        let filename = args[2].clone();

        Config { query, filename }
    }
}
