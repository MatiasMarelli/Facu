#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

typedef struct cond_{
  int under_cond;
  sem_t greencin; //sem_t sem; (oficialmente)
  pthread_mutex_t mut;
}cond;

void cond_init(cond* cond){ //los atributos te la dejo a vo : )
  sem_init(&(cond->greencin),0,0); //quiza aca puede ir el atributo del cond?
  cond->under_cond=0;
  pthread_mutex_init(&(cond->mut),NULL);
}

void cond_wait(cond* con,pthread_mutex_t* mut){
  //tecnicamente el mutex aqui deberia estar locked, lo asumiremos

  //no se si al hacer un broadcast, deberia detener a todos los threads hasta que se envie la "señal" a todos los threads
  //es decir que se levanten cantidad de threads veces el sema
  con->under_cond++;
  pthread_mutex_unlock(mut);
  sem_wait(con->greencin);
  pthread_mutex_lock(mut);
}

void cond_signal(cond* con){
  pthread_mutex_lock(&(con->mut));
  if(con->under_cond>0){
    con->under_cond--;
    sem_post(&(con->greencin));
  }
  pthread_mutex_unlock(&(con->mut));
}

void cond_broadcast(cond* con){
  pthread_mutex_lock(&(con->mut));

  for(int i =0;i<con->under_cond;i++)
    sem_post(&(con->greencin));
  con->under_cond=0;
  pthread_mutex_unlock(&(con->mut));
}

