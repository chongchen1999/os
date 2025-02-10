fn main() {
    let x = 10;
    let closure = || println!("x is: {}", x);
    closure(); // Uses x from outer scope

    println!("x is: {}", x);
}
