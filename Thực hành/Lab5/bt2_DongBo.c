#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define MAX_SIZE 100

int a[MAX_SIZE];
int count = 0;  

sem_t sem_add;     
sem_t sem_remove;  

void* producer(void* arg) {
    while (1) {
        int num = rand() % 100;  
        sem_wait(&sem_add);
        if (count < MAX_SIZE) {
            a[count++] = num;
            printf("Added: %d, Total elements: %d\n", num, count);
        }
        sem_post(&sem_remove);  
        usleep(100000); 
    }
}

void* consumer(void* arg) {
    while (1) {
        sem_wait(&sem_remove);
        if (count > 0) {
            int index = rand() % count;
            int removed = a[index];
            a[index] = a[--count];
            printf("Removed: %d, Remaining elements: %d\n", removed, count);
        } else {
            printf("Nothing in array a\n");
        }
        sem_post(&sem_add);  
        usleep(150000); 
    }
}

int main() {
    pthread_t producer_thread, consumer_thread;
    srand(time(NULL));

    sem_init(&sem_add, 0, MAX_SIZE);  
    sem_init(&sem_remove, 0, 0);      

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    sem_destroy(&sem_add);
    sem_destroy(&sem_remove);

    return 0;
}
