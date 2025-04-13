#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>



#define visitantes 4000000

int f1=0;
int f2=0;
int turno=0;

int visitas=0;

void * entrada(void* arg){
  for(int i=0; i < visitantes/2; i++){
    f1=1;
    turno=2;
    //asm("mfence");
    while(f2 && turno == 2){
    }
    visitas ++;
   // printf("%d\n",i);
    f1=0;
    
  }
}

void* salida (void* arg){
  for(int i=0; i<visitantes/2; i++){
    f2=1;
    turno=1;
    //asm("mfence");
    while(f1 && turno==1){
    }
    visitas++;
    f2=0;
    //printf("%d\n",i);
  }
}

int main(){
  pthread_t h1,h2;
  pthread_create(&h1,NULL,entrada,NULL);
  pthread_create(&h2,NULL,salida,NULL);
    
    //printf("Main joinning...\n");
  pthread_join(h1,NULL);
  pthread_join(h2,NULL);
  printf("Cantidad visitantes: %d\n",visitas);

  return 0;

}