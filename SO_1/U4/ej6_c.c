#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t tabaco, papel, fosforos, otra_vez;
int taba,pap,fosf;
pthread_cond_t t,p,f;
pthread_mutex_t mut =PTHREAD_MUTEX_INITIALIZER;

void agente(){

  while (1) {
    sem_wait(&otra_vez);
    int caso = random() % 3;
    if (caso != 0) sem_post(&fosforos);
    if (caso != 1) sem_post(&papel);
    if (caso != 2) sem_post(&tabaco);
  }
}
void fumar(int fumador){

  printf("Fumador %d: Puf! Puf! Puf!\n", fumador);
  sleep(1);
}
void * fumador1(void *arg){

  while (1) {
    /* sem_wait(&tabaco);
    sem_wait(&papel); */
    // ETE tiene FOSFORO
    pthread_mutex_lock(&mut);
  
    pthread_cond_wait(&f,&mut);
  
    pthread_mutex_unlock(&mut);

    fumar(1);
    sem_post(&otra_vez);
  }
}
void * fumador2(void *arg){

  while (1) {
    /* sem_wait(&fosforos);
    sem_wait(&tabaco); */
    // ETE tiene PAPEL
    pthread_mutex_lock(&mut);
    
    pthread_cond_wait(&p,&mut);
    
    pthread_mutex_unlock(&mut);
    fumar(2);
    sem_post(&otra_vez);
  }
}
void * fumador3(void *arg){

  while (1) {
    /* sem_wait(&papel);
    sem_wait(&fosforos); */

    //ETE tiene TABACO
    pthread_mutex_lock(&mut);
    
    pthread_cond_wait(&t,&mut);

    pthread_mutex_unlock(&mut);

    fumar(3);
    sem_post(&otra_vez);
  }
}

void* pusher_a (){
  while(1){
    sem_wait(&tabaco);
    pthread_mutex_lock(&mut);
    if(pap){
      pap=0;
      pthread_cond_signal(&f);
    }else if(fosf){
      fosf=0;
      pthread_cond_signal(&p);
    }else
      taba=1;
    pthread_mutex_unlock(&mut);
  }
  return NULL;
}
void* pusher_b (){
  while(1){
    sem_wait(&fosforos);
    pthread_mutex_lock(&mut);
    if(pap){
      pap=0;
      pthread_cond_signal(&f);
    }else if(taba){
      taba=0;
      pthread_cond_signal(&t);
    }else
      fosf=1;
    pthread_mutex_unlock(&mut);
  }
  return NULL;
}

void* pusher_c (){
  while(1){
    sem_wait(&papel);
    pthread_mutex_lock(&mut);
    if(taba){
      taba=0;
      pthread_cond_signal(&t);
    }else if(fosf){
      fosf=0;
      pthread_cond_signal(&p);
    }else
      pap=1;
    pthread_mutex_unlock(&mut);
  }
  return NULL;
}


int main(){

  pthread_t s1,s2,s3,p1,p2,p3;
  taba=fosf=pap=0;

  sem_init(&tabaco, 0, 0);
  sem_init(&papel, 0, 0);
  sem_init(&fosforos, 0, 0);
  sem_init(&otra_vez, 0, 1);
  pthread_create(&s1, NULL, fumador1, NULL);
  pthread_create(&s2, NULL, fumador2, NULL);
  pthread_create(&s3, NULL, fumador3, NULL);

  pthread_create(&p1, NULL, pusher_a, NULL);
  pthread_create(&p2, NULL, pusher_b, NULL);
  pthread_create(&p3, NULL, pusher_c, NULL);

  agente();
  return 0;
}
