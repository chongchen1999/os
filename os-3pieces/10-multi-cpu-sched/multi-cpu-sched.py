import threading
import queue
import time
import random

class Job:
    def __init__(self, job_id, execution_time):
        self.job_id = job_id
        self.execution_time = execution_time
        self.remaining_time = execution_time

    def run(self):
        time.sleep(0.1)  # Simulate execution
        self.remaining_time -= 1
        return self.remaining_time <= 0  # Return True if job is complete

class SingleQueueScheduler:
    def __init__(self, num_cpus):
        self.job_queue = queue.Queue()
        self.num_cpus = num_cpus
        self.lock = threading.Lock()

    def add_job(self, job):
        self.job_queue.put(job)

    def worker(self):
        while not self.job_queue.empty():
            job = self.job_queue.get()
            while not job.run():
                pass  # Continue executing job until completion
            print(f"Job {job.job_id} completed on {threading.current_thread().name}")

    def run(self):
        threads = [threading.Thread(target=self.worker, name=f"CPU-{i}") for i in range(self.num_cpus)]
        for t in threads:
            t.start()
        for t in threads:
            t.join()

class MultiQueueScheduler:
    def __init__(self, num_cpus):
        self.num_cpus = num_cpus
        self.queues = [queue.Queue() for _ in range(num_cpus)]
        self.locks = [threading.Lock() for _ in range(num_cpus)]

    def add_job(self, job):
        min_queue = min(self.queues, key=lambda q: q.qsize())
        min_queue.put(job)

    def worker(self, cpu_id):
        while not self.queues[cpu_id].empty():
            job = self.queues[cpu_id].get()
            while not job.run():
                pass
            print(f"Job {job.job_id} completed on CPU-{cpu_id}")

    def run(self):
        threads = [threading.Thread(target=self.worker, args=(i,)) for i in range(self.num_cpus)]
        for t in threads:
            t.start()
        for t in threads:
            t.join()

# Example usage
if __name__ == "__main__":
    num_cpus = 4
    jobs = [Job(i, random.randint(3, 6)) for i in range(10)]

    print("Running Single Queue Scheduler")
    sqs = SingleQueueScheduler(num_cpus)
    for job in jobs:
        sqs.add_job(job)
    sqs.run()

    print("\nRunning Multi Queue Scheduler")
    mqs = MultiQueueScheduler(num_cpus)
    for job in jobs:
        mqs.add_job(job)
    mqs.run()
