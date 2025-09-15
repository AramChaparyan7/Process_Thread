#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int size;
    int* arr;
} Args;

void* sum(void* v) {
    int* res = (int*)malloc(sizeof(int));
    *res = 0;
    Args* args = (Args*)v;
    for(int i = 0; i < args->size; ++i) {
        *res += args->arr[i];
    }
    return res;
}

int main() {
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    pthread_t th1;
    Args args;
    args.size = 10;
    args.arr = arr;
    int* res = NULL;
    pthread_create(&th1, NULL, sum, (void*)&args);
    pthread_join(th1, (void**)&res);
    printf("%d\n", *res);
    free(res);
    return 0;
}