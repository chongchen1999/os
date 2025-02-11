use std::sync::{Arc, Mutex, Condvar};
use std::thread;
use std::time::Duration;

fn waiter(shared: Arc<(Mutex<bool>, Condvar)>) {
    let (lock, cond) = &*shared;
    let mut ready = lock.lock().unwrap();
    while !*ready {
        println!("Waiter {:?}: Waiting...", thread::current().id());
        ready = cond.wait(ready).unwrap();
    }
    println!("Waiter {:?}: Proceeding...", thread::current().id());
}

fn sender(shared: Arc<(Mutex<bool>, Condvar)>) {
    thread::sleep(Duration::from_secs(2));
    let (lock, cond) = &*shared;
    *lock.lock().unwrap() = true;
    println!("Sender {:?}: Signaling...", thread::current().id());
    cond.notify_all();
}

fn main() {
    let shared = Arc::new((Mutex::new(false), Condvar::new()));
    let waiters: Vec<_> = (0..2).map(|_| {
        let shared = Arc::clone(&shared);
        thread::spawn(move || waiter(shared))
    }).collect();
    
    thread::spawn({
        let shared = Arc::clone(&shared);
        move || sender(shared)
    }).join().unwrap();
    
    for w in waiters { w.join().unwrap(); }
}
