#include <stdio.h>
#include <unistd.h>

int main(){
    printf("mian: %d\n" ,getpid());
    execl("./test","./test",NULL);
    return 0;
}