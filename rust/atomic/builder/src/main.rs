use std::thread::Builder;

fn main() {
    let numbers = Vec::from_iter(0..=1000);

    let t = Builder::new()
        .spawn(move || {
            let len = numbers.len();
            let sum = numbers.into_iter().sum::<usize>();

            sum / len
        })
        .unwrap();

    match t.join() {
        Ok(average) => println!("Average: {average}"),
        Err(e) => panic!("panic ! {e:?}"),
    }
}
