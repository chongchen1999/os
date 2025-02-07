import queue
import random

# Define constants
NUM_QUEUES = 3  # Number of priority levels
TIME_QUANTUMS = [4, 8, 16]  # Time slice for each level
BOOST_INTERVAL = 20  # Time interval for priority boost

class Process:
    def __init__(self, pid, burst_time):
        self.pid = pid
        self.burst_time = burst_time
        self.remaining_time = burst_time
        self.queue_level = 0  # Start at highest priority queue
    
    def __str__(self):
        return f"Process {self.pid}: remaining={self.remaining_time}, level={self.queue_level}"

def mlfq_scheduler(processes):
    queues = [queue.Queue() for _ in range(NUM_QUEUES)]
    time_elapsed = 0
    
    # Enqueue all processes in the highest priority queue initially
    for process in processes:
        queues[0].put(process)
    
    while any(not q.empty() for q in queues):
        for level in range(NUM_QUEUES):
            time_quantum = TIME_QUANTUMS[level]
            
            if queues[level].empty():
                continue
            
            process = queues[level].get()
            print(f"Time {time_elapsed}: Running {process} from queue {level}")
            
            if process.remaining_time > time_quantum:
                process.remaining_time -= time_quantum
                time_elapsed += time_quantum
                next_level = min(level + 1, NUM_QUEUES - 1)
                process.queue_level = next_level
                queues[next_level].put(process)
            else:
                time_elapsed += process.remaining_time
                process.remaining_time = 0
                print(f"Time {time_elapsed}: Process {process.pid} finished execution")
            
            # Priority Boost
            if time_elapsed % BOOST_INTERVAL == 0:
                print("Priority Boost: Moving all processes to top queue")
                for q_level in range(1, NUM_QUEUES):
                    while not queues[q_level].empty():
                        boosted_process = queues[q_level].get()
                        boosted_process.queue_level = 0
                        queues[0].put(boosted_process)

# Example usage
if __name__ == "__main__":
    process_list = [Process(i, random.randint(5, 20)) for i in range(5)]
    for process in process_list:
        print("process {} has a burst time of {}".format(process.pid, process.burst_time))
    mlfq_scheduler(process_list)