#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX_SIZE 100

int a[MAX_SIZE];
int count = 0;  

void* producer(void* arg) {
    while (1) {
        int num = rand() % 100; 
        if (count < MAX_SIZE) {
            a[count++] = num;
            printf("Added: %d, Total elements: %d\n", num, count);
        }
        usleep(100000); 
    }
}

void* consumer(void* arg) {
    while (1) {
        if (count > 0) {
            int index = rand() % count;
            int removed = a[index];
            a[index] = a[--count];
            printf("Removed: %d, Remaining elements: %d\n", removed, count);
        } else {
            printf("Nothing in array a\n");
        }
        usleep(150000);
    }
}

int main() {
    pthread_t producer_thread, consumer_thread;
    srand(time(NULL));

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    return 0;
}
