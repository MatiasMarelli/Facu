#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(){
    int fd;
    fd=open("Luis.txt",'r');
    char* ivovo= malloc(sizeof(char)*1024);
    int bytes=read(fd,ivovo,400);
    printf("%d\n",bytes);
    printf("%s\n",ivovo);

    return 0;
}