#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>

// El fin de fork :(

void* hola(void* p){
  printf("HELLO WORLD\n");

  pthread_exit(p);
  //*(int*)p=2;
  //return p;
}



int main(){
  pthread_t hilo;
  int p=1;

  pthread_create(&hilo,NULL,hola,&p);
  
  void* a;

  pthread_join(hilo,&a);

  printf("RETVALUE:%d\n",*(int*)a);

  return 0;
}