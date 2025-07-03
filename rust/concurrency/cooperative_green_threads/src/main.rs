mod green;

fn main() {
    green::spawn_from_main(gaia, 2 * 1024 * 1024); // 2 MiB
}

fn ortega() {
    for _ in 0..10 {
        println!("Call ortega");
        green::schedule();
    }
}

fn mash() {
    green::spawn(ortega, 2 * 1024 * 1024);

    for _ in 0..10 {
        println!("Call mash");
        green::schedule();
    }
}

fn gaia() {
    green::spawn(mash, 2 * 1024 * 1024);

    for _ in 0..10 {
        println!("Call gaia");
        green::schedule();
    }
}
