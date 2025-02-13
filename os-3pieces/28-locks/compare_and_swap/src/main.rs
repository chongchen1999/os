use std::sync::atomic::{AtomicI32, Ordering};

static GLOBAL: AtomicI32 = AtomicI32::new(0);

fn compare_and_swap(ptr: &AtomicI32, old: i32, new: i32) -> u8 {
    ptr.compare_exchange(old, new, Ordering::SeqCst, Ordering::SeqCst)
        .is_ok() as u8
}

fn main() {
    println!("before successful cas: {}", GLOBAL.load(Ordering::Relaxed));
    let success = compare_and_swap(&GLOBAL, 0, 100);
    println!(
        "after successful cas: {} (success: {})",
        GLOBAL.load(Ordering::Relaxed),
        success
    );

    println!("before failing cas: {}", GLOBAL.load(Ordering::Relaxed));
    let success = compare_and_swap(&GLOBAL, 0, 200);
    println!(
        "after failing cas: {} (old: {})",
        GLOBAL.load(Ordering::Relaxed),
        success
    );
}
