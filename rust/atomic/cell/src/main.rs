use std::cell::Cell;

fn main() {
    let a = Cell::new(3);
    let b = Cell::new(5);
    let c = Cell::new(vec![8]);

    f(&a, &b);
    safe_f(&c);
}

fn f(a: &Cell<i32>, b: &Cell<i32>) {
    let before = a.get();

    b.set(b.get() + 1);

    let after = a.get();

    if before != after {
        x();
    }
}

fn safe_f(v: &Cell<Vec<i32>>) {
    let mut v0 = v.take();

    v0.push(1);

    v.set(v0);
}

fn x() {
    println!("Not called ... But, called !");
}
