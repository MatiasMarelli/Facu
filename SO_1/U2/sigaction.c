#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void handler_child(int signum){
  printf("\nUN PENDEX\n");
  exit(0);
}

int main(){

  int arx;
  void (*signalreturn)(int);

  pid_t pid=fork();
  
  if(pid==0){
    struct sigaction sa;
    sa.sa_flags=SA_RESTART;
    sa.sa_handler=handler_child;
    printf("child....\n");
    sigaction(SIGINT,&sa,NULL);
    while(1);
  }
  if(pid != 0){
    sleep(1);
    kill(pid,SIGINT);
    wait(NULL);
    printf("termino el nene\n");
  }

  return 0;
}