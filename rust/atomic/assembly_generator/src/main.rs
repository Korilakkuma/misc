fn main() {
    let mut n = 2;

    add(&mut n);

    println!("add = {}", n);
}

fn add(num: &mut i32) {
    *num += 10
}
