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
    char* cptr = (char*)(m);
    *cptr = 0;
    pid_t pid = fork();
    if(pid < 0) {
        return 1;
    }
    else if(pid == 0) {
        while(1) {
            if(*cptr == 1) {
                break;
            }
        }
        printf("%s", cptr + 1);
    }
    else {
        strcpy(cptr + 1, "Hello from parent");
        *cptr = 1;
        wait(NULL);
    }
    fclose(f);
    return 0;
}
