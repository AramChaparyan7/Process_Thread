#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int arr[3][3] = {{1, 2, 3}, {4, 5, 6}, {6, 7, 9}};
    int p = fork();
    if(p < 0) {
        return 1;
    }
    else if(p == 0) {
        printf("child 1: ");
        int sum = 0;
        for(int i = 0; i < 3; ++i) {
            for(int j = 0; j < 3; ++j) {
                sum += arr[i][j];
            }
        }
        printf("%d \n", sum);
    }
    else {
        p = fork();
        if(p < 0) {
            return 1;
        }
        else if(p == 0) {
            printf("child 2: ");
            int sum = 0;
            for(int i = 0; i < 3; ++i) {
                sum += arr[i][i];
            }
            printf("%d \n", sum);
        }
        else {
            p = fork();
            if(p < 0) {
                return 1;
            }
            else if(p == 0) {
                printf("child 3: ");
                for(int i = 0; i < 3; ++i) {
                    for(int j = i; j < 3; ++j) {
                        printf("%d ", arr[i][j]);
                    }
                }
                printf("\n");
            }
            else {
                wait(NULL);
                wait(NULL);
                wait(NULL);
            }
        }
    }
    return 0;
}