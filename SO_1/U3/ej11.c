#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>






#define visitantes 10000000

int* f;


int visitas=0;

int cas (volatile __uint32_t* dest,int src,int a){
  int ret;
  asm("movl %1 , %%eax\n\t"
    "movl %2, %%ebx\n\t"
    "lock; cmpxchgl %%ebx, %0 "
    :
    "+m"(dest),"=m"(ret):"m"(src),"m"(a)
    :"eax","ebx","memory");
  return (ret == a);
}

void * entrada(void* arg){
  for(int i=0; i < visitantes/2; i++){
    *f=1;
    while(cas(f,1,2)){;}
    visitas ++;
    //printf("M1: %d\n",visitas);
    //cas(f,1,2);
    *f=2;
  }
}

void* salida (void* arg){
  for(int i=0; i<visitantes/2; i++){
    *f=2;
    while(cas(f,1,2)){;}
    visitas++;
    //printf("M2: %d\n",visitas);
    
    *f=1;
  }
}

int main(){
  f=malloc(sizeof(int));
  *f=0;
  pthread_t h1,h2;
  pthread_create(&h1,NULL,entrada,NULL);
  pthread_create(&h2,NULL,salida,NULL);
    
    //printf("Main joinning...\n");
  pthread_join(h1,NULL);
  pthread_join(h2,NULL);
  printf("Cantidad visitantes: %d\n",visitas);
  
  return 0;

}