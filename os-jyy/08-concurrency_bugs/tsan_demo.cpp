#include <iostream>
#include <thread>

int counter = 0; // 全局变量

void increment() {
    for (int i = 0; i < 100000; ++i) {
        ++counter; // 对 counter 进行写操作
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    std::cout << "Final counter value: " << counter << std::endl;
    return 0;
}