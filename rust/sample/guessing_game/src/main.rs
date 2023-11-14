use std::io;
use std::cmp::Ordering;
use rand::Rng;

fn main() {
    println!("Guess the number!");

    let secret_number = rand::thread_rng().gen_range(1..10);

    println!("The secret number is: {}", secret_number);

    println!("Type number > ");

    let mut guess_string = String::new();

    io::stdin()
        .read_line(&mut guess_string)
        .expect("Failed to read line");

    let guess: u32 = guess_string.trim().parse()
        .expect("Type a number > ");

    println!("guessed: {}", guess);

    match guess.cmp(&secret_number) {
        Ordering::Less => println!("Small < {}", secret_number),
        Ordering::Greater => println!("Big > {}", secret_number),
        Ordering::Equal => println!("Equal == {}", secret_number)
    }
}
