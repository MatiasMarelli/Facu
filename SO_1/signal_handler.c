#include <stdio.h>
#include <signal.h>

void handler_zero_division(int signum){

    printf("Div x 0 xd\n");
    exit(0);
}


int main(){
    void (*signalReturn)(int);
    signalReturn =signal(SIGFPE,handler_zero_division); 


    int a = 1/0;

    return 0;
}