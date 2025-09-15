#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int size;
    int* arr;
} Args;

int is_prime(int n) {
    for(int i = 2; i < n; ++i) {
        if(n%i == 0) {
            return 0;
        }
    }
    return 1;
}

int primecount(int n) {
    int res = 0;
    for(int i = 2; i < n; ++i) {
        if(is_prime(i)) {
            ++res;
        }
    }
    return res;
}

void* foo(void* v) {
    int* n = (int*)v;
    int* res = (int*)malloc(sizeof(int) * *n);
    int j = 0;
    for(int i = 2;i < *n; ++i) {
        if(is_prime(i)) {
            res[j] = i;
            ++j;
        }
    }
    return res;
}

void* f(void* v) {
    Args* args = (Args*)v;
    for(int i = 0; i < args->size; ++i) {
        printf("%d ", args->arr[i]);
    }
    free(args->arr);
    return NULL;
}

int main() {
    pthread_t th1, th2;
    int* res = NULL;
    int arg1 = 20;
    Args arg2;
    pthread_create(&th1, NULL, foo, (void*)&arg1);
    pthread_join(th1, (void**)&res);
    arg2.arr = res;
    arg2.size = primecount(20);
    pthread_create(&th2, NULL, f, (void*)&arg2);
    pthread_join(th1, (void**)&res);
    return 0;
}