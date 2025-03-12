#include <stdio.h>
#include <unistd.h>

int main(){
    pid_t pid=fork();
    if(pid==0){
        printf("child: %d, fork: %d\n",getpid(),pid);
    }else
    {
        printf("padre: %d , fork: %d\n",getpid(),pid);
    }
    
    return 0;

}