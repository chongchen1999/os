#include <pthread.h>
#include <stdio.h>

// 定义状态变量
int state = 0;
// 定义互斥锁
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// 定义条件变量
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// 打印<的线程函数
void* print_left(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (state!= 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("<");
        state = 1;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// 打印>的线程函数
void* print_right(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (state!= 1) {
            pthread_cond_wait(&cond, &mutex);
        }
        printf(">");
        if (state == 1) {
            state = 2;
        } else {
            state = 0;
        }
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// 打印_的线程函数
void* print_underscore(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (state!= 2) {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("_");
        state = 0;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}


int main() {
    pthread_t left_thread, right_thread, underscore_thread;

    // 创建打印<的线程
    pthread_create(&left_thread, NULL, print_left, NULL);
    // 创建打印>的线程
    pthread_create(&right_thread, NULL, print_right, NULL);
    // 创建打印_的线程
    pthread_create(&underscore_thread, NULL, print_underscore, NULL);

    // 等待线程结束（这里实际上不会结束，因为线程内部是无限循环）
    pthread_join(left_thread, NULL);
    pthread_join(right_thread, NULL);
    pthread_join(underscore_thread, NULL);

    // 销毁互斥锁和条件变量
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}