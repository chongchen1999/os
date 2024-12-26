#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static DEFINE_SPINLOCK(lockA);
static DEFINE_SPINLOCK(lockB);

static struct task_struct *thread1;
static struct task_struct *thread2;

static int thread_func1(void *data) {
    while (!kthread_should_stop()) {
        spin_lock(&lockA);
        pr_info("Thread 1 acquired lockA\n");
        msleep(100); // 模拟一些工作
        spin_lock(&lockB);
        pr_info("Thread 1 acquired lockB\n");
        // 临界区
        spin_unlock(&lockB);
        spin_unlock(&lockA);
        msleep(1000); // 模拟一些延迟
    }
    return 0;
}

static int thread_func2(void *data) {
    while (!kthread_should_stop()) {
        spin_lock(&lockB);
        pr_info("Thread 2 acquired lockB\n");
        msleep(100); // 模拟一些工作
        spin_lock(&lockA);
        pr_info("Thread 2 acquired lockA\n");
        // 临界区
        spin_unlock(&lockA);
        spin_unlock(&lockB);
        msleep(1000); // 模拟一些延迟
    }
    return 0;
}

static int __init lockdep_example_init(void) {
    pr_info("Lockdep example module loaded\n");

    // 创建内核线程
    thread1 = kthread_run(thread_func1, NULL, "lockdep_thread1");
    thread2 = kthread_run(thread_func2, NULL, "lockdep_thread2");

    if (IS_ERR(thread1) || IS_ERR(thread2)) {
        pr_err("Failed to create threads\n");
        return -1;
    }

    return 0;
}

static void __exit lockdep_example_exit(void) {
    // 停止线程
    if (thread1) kthread_stop(thread1);
    if (thread2) kthread_stop(thread2);

    pr_info("Lockdep example module unloaded\n");
}

module_init(lockdep_example_init);
module_exit(lockdep_example_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Lockdep Example Module");