#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <stdbool.h>

int P=false;
int M=false;

pthread_cond_t cv;
pthread_mutex_t lk=PTHREAD_MUTEX_INITIALIZER;



void* proc_a(void* data){
  
  if (!(P && M)){
    pthread_cond_wait(&cv,&lk);
  }  
  if(!(P && M)){
    printf ("MAAAANTEEEEECAAAAA\n");
  }else{
    printf("ñam ñam ñam...\n");
  }
  return NULL;
}

void* proc_b(void* data){
  
  pthread_mutex_lock(&lk);
  P = true;
  pthread_cond_signal(&cv);
  M = true;
  pthread_mutex_unlock(&lk);

  return NULL;
}



int main(){

  pthread_t a ,b;

  pthread_create(&a,NULL,proc_a,NULL);
  pthread_create(&b,NULL,proc_b,NULL);

  pthread_join(a,NULL);
  pthread_join(b,NULL);

  return 0;
}