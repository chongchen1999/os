import heapq

# Process structure
class Process:
    def __init__(self, pid, arrival_time, burst_time):
        self.pid = pid
        self.arrival_time = arrival_time
        self.burst_time = burst_time
        self.remaining_time = burst_time
        self.completion_time = 0
        self.response_time = -1
        self.start_time = -1

# First Come First Serve (FIFO)
def fifo_scheduling(processes):
    processes.sort(key=lambda x: x.arrival_time)
    current_time = 0
    total_turnaround, total_response = 0, 0

    for process in processes:
        if current_time < process.arrival_time:
            current_time = process.arrival_time
        process.start_time = current_time
        process.response_time = current_time - process.arrival_time
        process.completion_time = current_time + process.burst_time
        total_response += process.response_time
        total_turnaround += process.completion_time - process.arrival_time
        current_time += process.burst_time

    print("\nFIFO Scheduling:")
    print_metrics(processes, total_turnaround, total_response)

# Shortest Job First (SJF) - Non-preemptive
def sjf_scheduling(processes):
    processes.sort(key=lambda x: (x.arrival_time, x.burst_time))
    pq = []
    current_time = 0
    total_turnaround, total_response = 0, 0
    index = 0
    completed = 0

    while completed < len(processes):
        while index < len(processes) and processes[index].arrival_time <= current_time:
            heapq.heappush(pq, (processes[index].burst_time, index, processes[index]))
            index += 1

        if pq:
            _, _, process = heapq.heappop(pq)
            if process.start_time == -1:
                process.start_time = current_time
                process.response_time = current_time - process.arrival_time
            process.completion_time = current_time + process.burst_time
            total_response += process.response_time
            total_turnaround += process.completion_time - process.arrival_time
            current_time += process.burst_time
            completed += 1
        else:
            current_time += 1

    print("\nSJF Scheduling:")
    print_metrics(processes, total_turnaround, total_response)

# Round Robin (RR)
def rr_scheduling(processes, time_quantum=1):
    queue = []
    current_time = 0
    total_turnaround, total_response = 0, 0
    processes.sort(key=lambda x: x.arrival_time)
    index = 0
    completed = 0

    while completed < len(processes):
        while index < len(processes) and processes[index].arrival_time <= current_time:
            queue.append(processes[index])
            index += 1

        if queue:
            process = queue.pop(0)
            if process.start_time == -1:
                process.start_time = current_time
                process.response_time = current_time - process.arrival_time
                total_response += process.response_time

            execution_time = min(time_quantum, process.remaining_time)
            process.remaining_time -= execution_time
            current_time += execution_time

            if process.remaining_time == 0:
                process.completion_time = current_time
                total_turnaround += process.completion_time - process.arrival_time
                completed += 1
            else:
                queue.append(process)
        else:
            current_time += 1

    print("\nRound Robin Scheduling (Quantum={}):".format(time_quantum))
    print_metrics(processes, total_turnaround, total_response)

# Print results
def print_metrics(processes, total_turnaround, total_response):
    n = len(processes)
    print("PID | Arrival | Burst | Completion | Turnaround | Response")
    for p in processes:
        print(f"{p.pid:3} | {p.arrival_time:7} | {p.burst_time:5} | {p.completion_time:10} | {p.completion_time - p.arrival_time:10} | {p.response_time:8}")
    print(f"Average Turnaround Time: {total_turnaround/n:.2f}")
    print(f"Average Response Time: {total_response/n:.2f}")

# Test with example processes
process_list = [
    Process(1, 0, 5),
    Process(2, 2, 3),
    Process(3, 4, 1)
]

fifo_scheduling(process_list)
sjf_scheduling([Process(p.pid, p.arrival_time, p.burst_time) for p in process_list])  # Reset state
rr_scheduling([Process(p.pid, p.arrival_time, p.burst_time) for p in process_list], time_quantum=2)
