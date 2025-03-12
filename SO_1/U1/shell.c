#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int main(){
    int a;
    char* buffer = malloc(sizeof(char)*100);
    int flag = 1;
    pid_t pid=fork();
    int fd
    if(pid==0){
        scanf("%[^\n]",buffer);
        fd=open("shell.txt",'w');
        write(fd,buffer,100);
        sleep(1);
    }
    else{
        wait(&a)
        fd=open("shell.txt",'r');
        read(fd,buffer,100);
        strtok(buffer,' ');
        strcmp(buffer,"hola\n");
        if(strcmp(buffer,"hola\n")) execl("./test","./test",NULL);
        else {printf("Hola mundown");}
    }
    if(pid>0 && !strcmp(buffer,"exit")){flag=0;}
    return 0;
}