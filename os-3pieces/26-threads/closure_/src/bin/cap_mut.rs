fn main() {
    let mut counter = 0;
    let mut increment = || counter += 1;
    
    increment();
    increment();
    
    println!("Counter: {}", counter); // Output: 2
}
