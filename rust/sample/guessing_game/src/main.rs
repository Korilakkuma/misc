use rand::Rng;
use std::cmp::Ordering;
use std::io;

fn main() {
    println!("Guess the number!");

    let secret_number = rand::thread_rng().gen_range(1..10);

    println!("The secret number is: {}", secret_number);

    println!("Type number > ");

    loop {
        let mut guess_string = String::new();

        io::stdin()
            .read_line(&mut guess_string)
            .expect("Failed to read line");

        let guess: u32 = match guess_string.trim().parse() {
            Ok(num) => num,
            Err(_) => {
                println!("Type number > ");
                continue;
            }
        };

        println!("guessed: {}", guess);

        match guess.cmp(&secret_number) {
            Ordering::Less => println!("Small < {}", secret_number),
            Ordering::Greater => println!("Big > {}", secret_number),
            Ordering::Equal => {
                println!("Equal == {}", secret_number);
                break;
            }
        }
    }
}
