use std::cell::RefCell;

fn main() {
    let a = RefCell::new(vec![]);

    f(&a);

    dbg!(&a);
}

fn f(v: &RefCell<Vec<i32>>) {
    v.borrow_mut().push(1);
}
