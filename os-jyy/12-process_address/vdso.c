#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

double gettime() {
    struct timeval t;
    gettimeofday(&t, NULL);  // trapless system call
    return t.tv_sec + t.tv_usec / 1000000.0;
}

int main() {
    printf("Time stamp: %ld\n", time(NULL));  // trapless system call
    double st = gettime();
    sleep(1);
    double ed = gettime();
    printf("Time: %.6lfs\n", ed - st);
}