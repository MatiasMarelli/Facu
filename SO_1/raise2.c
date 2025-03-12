#include <stdio.h>
#include <signal.h>

int main(){
    raise(SIGSTOP);

    printf("B\n");
    return 0;
}