#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int x = 0;
pthread_mutex_t lock; 

void* processA(void* arg) {
    while (1) {
        pthread_mutex_lock(&lock);  
        x = x + 1;
        if (x == 20) {
            x = 0;
        }
        printf("Process A: x = %d\n", x);
        pthread_mutex_unlock(&lock);  
        usleep(100000);  
    }
}

void* processB(void* arg) {
    while (1) {
        pthread_mutex_lock(&lock); 
        x = x + 1;
        if (x == 20) {
            x = 0;
        }
        printf("Process B: x = %d\n", x);
        pthread_mutex_unlock(&lock); 
        usleep(100000);  
    }
}

int main() {
    pthread_t threadA, threadB;
    pthread_mutex_init(&lock, NULL);  

    // Tạo hai thread tương ứng với Process A và Process B
    pthread_create(&threadA, NULL, processA, NULL);
    pthread_create(&threadB, NULL, processB, NULL);

    // Chờ cả hai thread kết thúc (thực tế là không bao giờ vì vòng lặp vô hạn)
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);

    pthread_mutex_destroy(&lock);  

    return 0;
}
