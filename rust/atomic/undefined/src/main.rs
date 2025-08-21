fn main() {
    let a = 3;
    let mut b = 5;

    f(&a, &mut b);
}

fn f(a: &i32, b: &mut i32) {
    let before = *a;

    *b += 1;

    let after = *a;

    if before != after {
        x();
    }
}

fn x() {
    println!("Not called");
}
