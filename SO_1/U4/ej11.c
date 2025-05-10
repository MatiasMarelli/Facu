#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct sem{
  int max,count;
  pthread_cond_t cond;
  pthread_mutex_t mut;
}semaphore;

void sem_init(semaphore* sem,int n){
  sem->max=n;
  sem->count=0;
  pthread_mutex_init(&(sem->mut),NULL);
}

void sem_post(semaphore* sem){
  pthread_mutex_lock(&(sem->mut));
  if(sem->count==0){
    perror("Semaforo en 0");
    pthread_mutex_unlock(&(sem->mut));
    return;
  }
  sem->count--;

  if((sem->count) + 1 ==sem->max ){pthread_cond_signal(&(sem->cond));}

  pthread_mutex_unlock(&(sem->mut));
}

void sem_wait(semaphore* sem){
  pthread_mutex_lock(&(sem->mut));
  while(sem->max <= sem->count){
    pthread_cond_wait(&(sem->cond),&(sem->mut));
  }
  sem->count++;
  pthread_mutex_unlock(&(sem->mut));

}

void sem_destroy(semaphore* sem){
  pthread_mutex_destroy(&(sem->mut));
}