#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main() {
    pid_t pid1 = fork();
    if(pid1 < 0) {
        perror("Fork failed");
    }
    else if(pid1 == 0) {
        execlp("ls", "ls", NULL);
    } 
    else {
        pid_t pid2 = fork();
        if(pid2 < 0) {
            perror("Fork failed");
        }
        else if(pid2 == 0) {
            execlp("date", "date", NULL);
        } 
        else {
            pid_t pid3 = fork();
            if(pid3 < 0) {
                perror("Fork failed");
            }
            else if(pid3 == 0) {
                execlp("whoami", "whoami", NULL);
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