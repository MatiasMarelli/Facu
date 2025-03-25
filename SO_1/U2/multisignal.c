#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void handler(int signum){
  printf("\n");
  printf("Se preciono Ctrl + C\n");
  signal(SIGINT,SIG_DFL);
  printf("HOLA");  
}



int main(){
  
  void(*signalhandler)(int);

  signalhandler=signal(SIGINT,handler);
  while(1);
  //raise(SIGINT);

  //raise(SIGINT);
  //printf("Terminar");

  return 0;

}