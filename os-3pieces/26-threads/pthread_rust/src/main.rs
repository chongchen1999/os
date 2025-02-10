use std::thread;

fn mythread(arg: &str) {
    println!("{}", arg);
}

fn main() {
    println!("main: begin");

    let handle1 = thread::spawn(|| mythread("A"));
    let handle2 = thread::spawn(|| mythread("B"));

    handle1.join().expect("Thread 1 failed");
    handle2.join().expect("Thread 2 failed");

    println!("main: end");
}
