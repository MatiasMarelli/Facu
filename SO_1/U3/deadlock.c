#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>


pthread_mutex_t mutex1 =PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 =PTHREAD_MUTEX_INITIALIZER;


void* threadA (void* arg){
  pthread_mutex_lock(&mutex1);

  sleep(1);

  pthread_mutex_lock(&mutex2);

  //REGION CRITICA

  printf("HOOOLAAAA\n");

  pthread_mutex_unlock(&mutex2);
  pthread_mutex_unlock(&mutex1);

}

void* threadB (void* arg){

  pthread_mutex_lock(&mutex2);

  sleep(1);

  pthread_mutex_lock(&mutex1);

  //REGION CRITICA

  printf("HOOOLAAAA 2!\n");

  pthread_mutex_unlock(&mutex2);
  pthread_mutex_unlock(&mutex1);
}



int main(){

  //HOLA

  pthread_t h1,h2;
  pthread_create(&h1,NULL,threadA,NULL);
  pthread_create(&h2,NULL,threadB,NULL);
  
  //printf("Main joinning...\n");

  pthread_join(h1,NULL);
  pthread_join(h2,NULL);

  pthread_mutex_destroy(&mutex1);
  pthread_mutex_destroy(&mutex2);
  

  return 0;
}