#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#define MAX_THREADS 20

pthread_t th_arr[20];

typedef struct{
    int thread_id;
    int line_number;
    char* line_data;
    int word_count;
}  ThreadData;


void* process_line(void* v) {
    ThreadData* data = (ThreadData*)v;
    int flag = 1;
    for(int i = 0; data->line_data[i] != '\0'; ++i) {
        if(data->line_data[i] == ' ' || data->line_data[i] == '\n' || data->line_data[i] == '\t') {
            flag = 1;
        } 
        else if(flag) {
            flag = 0;
            data->word_count++;
        }
    }
    return NULL;
}

int main() {
    ThreadData* arr[1000];
    int id = 0;
    FILE* input = fopen("input.txt", "r");
    char* buffer = (char*)malloc(100 * sizeof(char));
    size_t size = 100;
    int count = 0;
    int line_count = 1;
    while(getline(&buffer, &size, input) != -1) {
        ThreadData* args = (ThreadData*)malloc(sizeof(ThreadData));
        char* buf = (char*)malloc(sizeof(char) * 100);
        memcpy(buf, buffer, 100);
        args->line_number = line_count;
        args->line_data = buf;
        args->thread_id = id;
        args->word_count = 0;
        arr[id] = args;
        ++id;
        ++line_count;
        pthread_create(&th_arr[count], NULL, process_line, (void*)args);
        ++count;
        if(count >= 20) {
            for(int i = 0; i < MAX_THREADS; ++i) {
                pthread_join(th_arr[i], NULL);
            }
            count  = 0;
        }
    }
    for(int i = 0; i < count; ++i) {
        pthread_join(th_arr[i], NULL);
    }
    count  = 0;
    int sum_words = 0;
    int max = 0;
    int max_id = 0;
    for(int i = 0; i < id; ++i) {
        sum_words += arr[i]->word_count;
        if(arr[i]->word_count > max) {
            max = arr[i]->word_count;
            max_id = arr[i]->line_number;
        }
        free(arr[i]->line_data);
        free(arr[i]);
    }
    free(buffer);
    fclose(input);
    printf("Word count is %d, max line is %d\n", sum_words, max_id);
    return 0;
}