#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(){
    int s;
    pid_t pid = fork();
    if(pid == 0){
        printf("soy el hijo, no conozco a mi papa :(\n");
        sleep(2);
    } else {
        wait(&s);
        printf("soy el papa, conozco a mi hijo :o\n");
    }   
}