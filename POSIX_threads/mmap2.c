#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <wait.h>
#include <string.h>


int main() {
    FILE* f = fopen("file.txt",  "r+");
    if(!f) {
        perror("file not opened");
        return 1;
    }
    int fd = fileno(f);
    ftruncate(fd, 4096);
    void* m = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(m == MAP_FAILED) {
        perror("mmap error");
        return 1;
    }
    int* iptr = (int*)(m);
    *iptr = 0;
    pid_t pid = fork();
    if(pid < 0) {
        return 1;
    }
    else if(pid == 0) {
        while(1) {
            if(*iptr == 1) {
                break;
            }
        }
        for(int i = 1; i <= 10; ++i) {
            printf("%d ", iptr[i]);
        }
        printf("\n");
    }
    else {
        for(int i = 1; i <= 10; ++i) {
            iptr[i] = i;
        }
        *iptr = 1;
        wait(NULL);
    }
    fclose(f);
    return 0;
}
