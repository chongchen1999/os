import time
import os

def system_call_simulation():
    """Simulates a system call by reading a file using OS system calls"""
    print("User process making a system call to read a file...")
    with open("/etc/hostname", "r") as file:  # This triggers a system call
        print("System Call Output:", file.read().strip())

def context_switch_simulation():
    """Simulates a simple context switch using multitasking"""
    pid = os.fork()  # Create a child process
    if pid == 0:
        print(f"Child Process {os.getpid()} running...")
        time.sleep(2)  # Simulate process execution
        print(f"Child Process {os.getpid()} completed.")
    else:
        print(f"Parent Process {os.getpid()} waiting for child...")
        os.wait()  # Wait for child to finish
        print("Parent resumes execution.")

# Simulate system call and context switching
system_call_simulation()
context_switch_simulation()
