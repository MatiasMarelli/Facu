
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>


#define visitantes 1000000

int visitas=0;

void * entrada(void* arg){
  for(int i=0; i < visitantes/2; i++){
    
    visitas ++;
    
  }
}

void* salida (void* arg){
  for(int i=0; i<visitantes/2; i++){
    visitas++;
    
    
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