use std::sync::atomic::{AtomicU32, Ordering};

fn atomic_increment(counter: &AtomicU32) {
    let mut current;
    loop {
        current = counter.load(Ordering::Acquire); // Load current value (LL equivalent)
        let new = current + 1; // Increment
        
        // Try to store (SC equivalent), ensuring atomicity
        if counter.compare_exchange(current, new, Ordering::AcqRel, Ordering::Acquire).is_ok() {
            break; // Success, exit loop
        }
        // If SC fails, retry
    }
}

fn main() {
    let counter = AtomicU32::new(0);

    println!("Counter before: {}", counter.load(Ordering::Relaxed));
    atomic_increment(&counter);
    println!("Counter after: {}", counter.load(Ordering::Relaxed));
}
