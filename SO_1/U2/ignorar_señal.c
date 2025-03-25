#include <stdio.h>
#include <signal.h>
#include <stdlib.h>


void handler(int signum){
  
}



int main(){
  void (*signalreturn)(int);

  signalreturn=signal(SIGTSTP,SIG_IGN);

  raise(SIGTSTP);

  printf("Ignorada\n");

  return 0;
}