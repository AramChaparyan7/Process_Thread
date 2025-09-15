#include <pthread.h>
#include <stdio.h>
#include <wait.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
    int size;
    int* arr;
    int ret;
} Args;

void* sum(void* v) {
    int res = 0;
    Args* args = (Args*)v;
    if(args->size == 0) {
        args->ret = 1;
        return NULL;
    }
    for(int i = 0; i < args->size; ++i) {
        res += args->arr[i];
    }
    printf("%d\n", res);
    return NULL;
}

int main() {
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    pthread_t th1;
    Args args;
    args.size = 10;
    args.arr = arr;
    args.ret = 0;
    int* res = NULL;
    pid_t pid = fork();
    if(pid < 0) { 
        perror("fork error");
        return 1;
    }
    else if (pid == 0) {
        pthread_create(&th1, NULL, sum, (void*)&args);
        pthread_join(th1, (void**)&res);
        if(args.ret == 1) {
            printf("size of array is 0\n");
        }
    }
    else {
        wait(NULL);

    }
    return 0;
}