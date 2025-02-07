# The Linux Completely Fair Scheduler (CFS)
from sortedcontainers import SortedKeyList
import time

class Process:
    def __init__(self, pid, nice=0):
        self.pid = pid
        self.nice = nice
        self.weight = self.get_weight(nice)
        self.vruntime = 0  # Virtual runtime
        self.runtime = 0  # Actual CPU time used

    def get_weight(self, nice):
        """ Maps nice values (-20 to +19) to weight values similar to Linux CFS """
        prio_to_weight = [
            88761, 71755, 56483, 46273, 36291, 29154, 23254, 18705, 14949, 11916,  # -20 to -11
            9548, 7620, 6100, 4904, 3906, 3121, 2501, 1991, 1586, 1277,            # -10 to -1
            1024, 820, 655, 526, 423, 335, 272, 215, 172, 137,                     # 0 to +9
            110, 87, 70, 56, 45, 36, 29, 23, 18, 15                                # +10 to +19
        ]
        return prio_to_weight[nice + 20]  # Normalize nice value to index range 0-39

    def update_vruntime(self, timeslice):
        """ Updates virtual runtime based on weight """
        self.runtime += timeslice
        self.vruntime += (1024 / self.weight) * timeslice  # Adjusted for priority

    def __repr__(self):
        return f"Process(pid={self.pid}, nice={self.nice}, vruntime={self.vruntime:.2f}, runtime={self.runtime})"

class CFSScheduler:
    def __init__(self):
        self.process_queue = SortedKeyList(key=lambda p: p.vruntime)  # Use SortedKeyList

    def add_process(self, process):
        """ Add a new process to the queue """
        self.process_queue.add(process)

    def pick_next_process(self):
        """ Pick the process with the lowest virtual runtime """
        return self.process_queue[0] if self.process_queue else None

    def run(self, total_time=100):
        """ Simulate scheduling over a given time period """
        time_slice = 10  # Default time slice per process

        while total_time > 0 and self.process_queue:
            current = self.pick_next_process()
            print(f"Running: {current}")

            actual_timeslice = min(time_slice, total_time)

            # Remove process, update vruntime, then reinsert it
            self.process_queue.remove(current)
            current.update_vruntime(actual_timeslice)
            self.process_queue.add(current)  # Reinsert after update

            total_time -= actual_timeslice
            time.sleep(0.5)  # Simulate execution time

        print("\nFinal Process States:")
        for p in self.process_queue:
            print(p)


# Example usage
scheduler = CFSScheduler()

# Add some processes with different priorities
scheduler.add_process(Process(pid=1, nice=-5))  # Higher priority
scheduler.add_process(Process(pid=2, nice=0))   # Default priority
scheduler.add_process(Process(pid=3, nice=10))  # Lower priority

# Run the scheduler simulation
scheduler.run(total_time=100)
