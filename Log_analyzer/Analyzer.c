#include <syscall.h>
#include <wait.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int count = 0;
    int error = 0;
    double sum = 0;
    int i = 1;
    for(; ;++i) {
        char name[10];
        snprintf(name, 10, "Log%d.txt", i);
        FILE* f = fopen(name, "r");
        if(!f) { break; }
        pid_t p = fork();
        if(p < 0) { exit(-1); }
        else if(p == 0) {
            int command = 0;
            int speed = 0;
            char newname[20];
            snprintf(newname, 20, "%d_result.json", i);
            FILE* out = fopen(newname, "w");
            if(!out) {  exit(-1);}
            int fcount = 0;
            int ferror = 0;
            double fsum = 0;
            while(fscanf(f, "%d %d", &command, &speed) == 2) {
                ++fcount;
                if(command == 500) {
                    ++ferror;
                }
                fsum += speed;
            }
            fprintf(out, "{\n");
            fprintf(out, "\"file\" : \"%s\"\n", name);
            fprintf(out, "\"total_requests\" : %d\n", fcount);
            fprintf(out, "\"https_500_error\" : %d\n", ferror);
            fprintf(out, "\"avg_time\" : %f\n", fsum/fcount);
            fprintf(out, "}\n");
            fclose(out);
            exit(0);
        }
        else {
            wait(NULL);
           
        }
        fclose(f);
    }
    return 0;
}