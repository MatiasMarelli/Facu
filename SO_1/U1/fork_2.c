#include <stdio.h>
#include <unistd.h>

int main(){

    pid_t pid1=fork();
    pid_t pid2=fork();

    printf("pid: %d, pid1: %d, pid2: %d\n",getpid(),pid1,pid2);

    return 0;
}
