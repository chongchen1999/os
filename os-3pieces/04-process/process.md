## The Abstraction: The Process

### What is a Process?
A process is a fundamental abstraction provided by the OS, defined as a running program. The OS loads a program from disk into memory and manages its execution.

### The Illusion of Many CPUs
- The OS enables multiple processes to run concurrently by **virtualizing the CPU**.
- This is achieved through **time-sharing**, where the CPU switches between processes, creating an illusion of parallel execution.

## Process Virtualization Mechanisms

### Mechanisms and Policies
- **Mechanisms**: Low-level functionalities such as context switching.
- **Policies**: Decision-making strategies like scheduling, determining which process runs next.

### Time Sharing vs. Space Sharing
- **Time Sharing**: Multiple processes take turns using a single CPU.
- **Space Sharing**: Resources (e.g., memory, disk space) are divided among processes.

## Process Components

A process consists of:
- **Memory (Address Space)**: Includes code, static data, heap, and stack.
- **Registers**: Stores current execution state, including the **Program Counter (PC)**.
- **I/O Information**: Open files and ongoing operations.

## Process API
Typical operations available in an OS for managing processes:
- **Create**: Launch a new process.
- **Destroy**: Terminate a process.
- **Wait**: Pause execution until a process finishes.
- **Control**: Suspend or resume a process.
- **Status**: Retrieve process details.

## Process Creation
- The OS loads program code and static data from disk into memory.
- Memory is allocated for:
  - **Stack**: Stores function calls and local variables.
  - **Heap**: Manages dynamically allocated memory.
- **I/O Setup**: Initializes file descriptors for input/output operations.

## Process States
A process can be in one of three main states:
1. **Running**: Actively executing on the CPU.
2. **Ready**: Waiting for CPU time.
3. **Blocked**: Waiting for an external event (e.g., I/O completion).

### State Transitions
Processes transition between states based on scheduling and I/O operations.

## Process Data Structures
The OS maintains a **Process Control Block (PCB)** containing:
- Process ID (PID)
- Memory and CPU state
- Open file descriptors
- Parent process reference

## Conclusion
The OS abstracts the CPU using process management techniques. By implementing virtualization mechanisms and intelligent scheduling policies, it allows multiple programs to run efficiently on limited hardware resources.
