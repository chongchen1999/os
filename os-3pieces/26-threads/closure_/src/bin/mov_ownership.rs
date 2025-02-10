fn main() {
    let s = String::from("Hello");
    
    let consume = move || println!("{}", s);
    
    consume(); // s is moved and can’t be used anymore

    // println!("{}", s);
}
