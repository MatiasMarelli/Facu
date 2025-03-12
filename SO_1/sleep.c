#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv){
    while (1){
        pid_t pid=fork();
        if(pid==0) {execl(argv[2],argv[2],NULL);}
        else sleep(atoi(argv[1]));
    
    }

    return 0;

}