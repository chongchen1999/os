# Virtual Memory Simulator in Rust

## Overview
This Rust program simulates a basic virtual memory system inspired by VAX/VMS and Linux memory management techniques. It demonstrates fundamental concepts such as:
- **Page Table and TLB (Translation Lookaside Buffer)** for efficient address translation
- **Page Fault Handling** to allocate new pages when needed
- **Page Replacement** using FIFO strategy when memory is full

## Features
- Simulates a small memory space (4KB) with 512-byte pages
- Implements a TLB for fast lookups
- Handles page faults and allocates pages dynamically
- Replaces pages using a simple FIFO algorithm

## How to Run
1. Ensure you have Rust installed. If not, install it from [Rust's official site](https://www.rust-lang.org/).
2. Clone or download this repository.
3. Navigate to the project directory and run:
   ```sh
   cargo run
   ```

## Example Output
```
Page Fault: VPN 0 not in memory
Allocating frame 0 to VPN 0
Page Fault: VPN 1 not in memory
Allocating frame 1 to VPN 1
Page Fault: VPN 2 not in memory
Allocating frame 2 to VPN 2
Page Fault: VPN 3 not in memory
Allocating frame 3 to VPN 3
Page Fault: VPN 4 not in memory
Evicting VPN 0 from PFN 0
Allocating frame 0 to VPN 4
```

## License
This project is licensed under the MIT License.

