use std::sync::{Arc, Mutex};
use std::thread;
use std::time::Duration;

fn thread_function(lock: Arc<Mutex<()>>, thread_id: u64) {
    let _guard = lock.lock().unwrap();  // Lock the mutex
    println!("Thread {} has entered the critical section.", thread_id);

    // Critical section
    thread::sleep(Duration::from_secs(1));

    // println!("The address of the lock is: {:p}", &(*lock));
    // println!("The reference count of the lock is: {}", Arc::strong_count(&lock) as u64);

    println!("Thread {} is leaving the critical section.", thread_id);
}

fn main() {
    // Create a Mutex
    let lock = Arc::new(Mutex::new(()));
    
    println!("The reference count of the lock is: {}", Arc::strong_count(&lock) as u64);

    // Display size of Mutex (for Rust's Mutex)
    println!("size of Mutex is: {}", std::mem::size_of::<Mutex<()>>());

    // Create two threads
    let t1_lock = Arc::clone(&lock);
    // println!("The reference count of the lock is: {}", Arc::strong_count(&lock) as u64);
    let t1 = thread::spawn(move || {
        thread_function(t1_lock, 1);
    });

    let t2_lock = Arc::clone(&lock);
    // println!("The reference count of the lock is: {}", Arc::strong_count(&lock) as u64);
    let t2 = thread::spawn(move || {
        thread_function(t2_lock, 2);
    });

    // Join the threads
    t1.join().unwrap();
    t2.join().unwrap();
}
