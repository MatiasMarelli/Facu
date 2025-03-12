#include <stdio.h>
#include <signal.h>

int main(){
    raise(SIGSTOP);

    printf("A\n");
    return 0;
}