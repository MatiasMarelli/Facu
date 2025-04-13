#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>

#define coso 1000000

/* 
static inline void* incl(int *p) {
  for(int i=0;i<coso/2;i++){
    asm("incl %0" : "+m"(*p) : : "memory");
  }
  return;
}

static inline void* incl2(int *p) {
  for(int i=0;i<coso/2;i++){
    asm("incl %0" : "+m"(*p) : : "memory");
  }
  return;
} */


//  VERSION ATOMICA DE INCREMENTAR EN 1 LA VARIABLE

static inline void incl(int *p) {
  for(int i=0;i<coso/2;i++){
    asm("lock ; incl %0" : "+m"(*p) : : "memory");  
  }
}

static inline void incl2(int *p) {
  for(int i=0;i<coso/2;i++){
    asm("lock ; incl %0" : "+m"(*p) : : "memory");  
  }
}
  


int main(){
  int* p;

  *p=0;
  
  pthread_t h1,h2;
  pthread_create(&h1,NULL,incl,p);
  pthread_create(&h2,NULL,incl2,p);
    
    //printf("Main joinning...\n");
  pthread_join(h1,NULL);
  pthread_join(h2,NULL);

  printf("%d\n",*p);

  return 0;
}