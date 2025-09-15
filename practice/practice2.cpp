#include <pthread.h>
#include <iostream>
#include <queue>
#include <string>

std::queue<std::string> q{};
int M = 10;
pthread_mutex_t mutex;
pthread_cond_t cond;

void* producer1(void*) {
    for(int i = 0; i < 4; ++i) {
        pthread_mutex_lock(&mutex);
        while(q.size() == 10) {
            pthread_cond_wait(&cond, &mutex);
        }
        q.push("Producer1");
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
    }
    return NULL;
}

void* producer2(void*) {
    for(int i = 0; i < 4; ++i) {
        pthread_mutex_lock(&mutex);
        while(q.size() == 10) {
            pthread_cond_wait(&cond, &mutex);
        }
        q.push("Producer2");
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
    }
    return NULL;
}

void* consumer(void*) {
    for(int i = 0; i < 8; ++i) {
        pthread_mutex_lock(&mutex);
        while(q.empty()) {
            pthread_cond_wait(&cond, &mutex);
        }
        std::cout << q.front() << std::endl;
        q.pop();
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
    }
    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_t th1, th2, th3;
    pthread_create(&th1, NULL, consumer, NULL);
    pthread_create(&th2, NULL, producer1, NULL);
    pthread_create(&th2, NULL, producer2, NULL);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}