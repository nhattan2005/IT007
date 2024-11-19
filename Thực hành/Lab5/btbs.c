#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int x1, x2, x3, x4, x5, x6;
int w, v, y, z, ans;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_v = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_yz = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_yz_calculated = PTHREAD_COND_INITIALIZER;

int v_calculated = 0;      
int y_and_z_calculated = 0;

void* calculate_w(void* arg) {
    pthread_mutex_lock(&lock);
    w = x1 * x2; 
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* calculate_v(void* arg) {
    pthread_mutex_lock(&lock);
    v = x3 * x4; 
    v_calculated = 1;
    pthread_cond_broadcast(&cond_v);
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* calculate_y(void* arg) {
    pthread_mutex_lock(&lock);
    // Chờ cho đến khi v được tính xong
    while (!v_calculated)
        pthread_cond_wait(&cond_v, &lock);

    y = v * x5; 
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* calculate_z(void* arg) {
    pthread_mutex_lock(&lock);
    // Chờ cho đến khi v được tính xong
    while (!v_calculated)
        pthread_cond_wait(&cond_v, &lock);

    z = v * x6; 
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* calculate_yz(void* arg) {
    pthread_mutex_lock(&lock);
    while (!v_calculated)
        pthread_cond_wait(&cond_v, &lock);

    y = w * y; 
    z = w * z;

    y_and_z_calculated = 1;
    pthread_cond_broadcast(&cond_yz_calculated);
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* calculate_ans(void* arg) {
    pthread_mutex_lock(&lock);
    while (!y_and_z_calculated)
        pthread_cond_wait(&cond_yz_calculated, &lock);

    ans = y + z; // (g)
    printf("Final answer (ans): %d\n", ans);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    x1 = 2; x2 = 3; x3 = 4; x4 = 5; x5 = 6; x6 = 7;

    pthread_t thread_w, thread_v, thread_y, thread_z, thread_yz, thread_ans;

    pthread_create(&thread_w, NULL, calculate_w, NULL);
    pthread_create(&thread_v, NULL, calculate_v, NULL);
    pthread_create(&thread_y, NULL, calculate_y, NULL);
    pthread_create(&thread_z, NULL, calculate_z, NULL);
    pthread_create(&thread_yz, NULL, calculate_yz, NULL);
    pthread_create(&thread_ans, NULL, calculate_ans, NULL);

    pthread_join(thread_w, NULL);
    pthread_join(thread_v, NULL);
    pthread_join(thread_y, NULL);
    pthread_join(thread_z, NULL);
    pthread_join(thread_yz, NULL);
    pthread_join(thread_ans, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond_v);
    pthread_cond_destroy(&cond_yz);
    pthread_cond_destroy(&cond_yz_calculated);

    return 0;
}
