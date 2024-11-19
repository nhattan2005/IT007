#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int sells = 0;    
int products = 0; 

sem_t sem_sell;     
sem_t sem_produce;   

void* processA(void* arg) {
    while (1) {
        sem_wait(&sem_sell);   
        sells++;
        printf("Sold: %d, Products: %d\n", sells, products);

        sem_post(&sem_produce);

        usleep(100000); 
    }
}

void* processB(void* arg) {
    while (1) {
        sem_wait(&sem_produce); 
        products++;
        printf("Produced: %d, Sells: %d\n", products, sells);

        sem_post(&sem_sell);

        usleep(100000); 
    }
}

int main() {
    pthread_t threadA, threadB;

    sem_init(&sem_sell, 0, 0);     
    sem_init(&sem_produce, 0, 1405); 
    pthread_create(&threadA, NULL, processA, NULL);
    pthread_create(&threadB, NULL, processB, NULL);

    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);

    sem_destroy(&sem_sell);
    sem_destroy(&sem_produce);

    return 0;
}
