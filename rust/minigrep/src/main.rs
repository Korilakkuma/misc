use std::env;
use std::fs::File;
use std::io::prelude::*;

fn main() {
    let args: Vec<String> = env::args().collect();

    let query = &args[1];
    let filename = &args[2];

    println!("Searching for {}", query);
    println!("in file {}", filename);

    let mut file = File::open(filename).expect("File is not found");

    let mut contents = String::new();

    file.read_to_string(&mut contents).expect("Error occurred in reading file");

    println!("File contents:\n{}", contents);
}
