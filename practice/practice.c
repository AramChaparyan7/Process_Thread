#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
int data;
int flag;

void* producer(void*) {
    pthread_mutex_lock(&mutex);
    while(flag == 1) {
        pthread_cond_wait(&cond, &mutex);
    }
    for(int i = 0; i< 10; ++i) {
        data = i;
    }
    flag = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* consumer(void*) {
    pthread_mutex_lock(&mutex);
    while(flag == 0) {
        pthread_cond_wait(&cond, &mutex);
    }
    printf("%d", data);
    flag = 0;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_t th1, th2;
    pthread_create(&th1, NULL, consumer, NULL);
    pthread_create(&th2, NULL, producer, NULL);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}