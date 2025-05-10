#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define M 10
#define N 10
#define ARRLEN 10240

int reading=0;int writing=0;

int arr[ARRLEN];

sem_t amarillito;
pthread_cond_t cv,non_write;
pthread_mutex_t escri=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t reader=PTHREAD_MUTEX_INITIALIZER;

void reader_lock(){

  pthread_mutex_lock(&reader);
    while(writing>0){
      pthread_cond_wait(&non_write,&reader);
    }
    reading++;
    pthread_mutex_unlock(&reader);
}

void reader_unlock(){
  pthread_mutex_lock(&reader)
  reading--;
  if (!reading) pthread_cond_signal(&cv);
    pthread_mutex_unlock(&reader);
}




void * escritor(void *arg){

  int i;
  int num = arg - (void*)0;
  while (1){
    sleep(random() % 3);
    pthread_mutex_lock(&escri);
    //sem_wait(&amarillito);
    while(reading>0){
      pthread_cond_wait(&cv,&escri);
    }
    writing++;
    printf("Escritor %d escribiendo\n", num);
    for (i = 0; i < ARRLEN; i++)
      arr[i] = num;
    
    printf("Escritor %d dejo de escribir\n", num);
    writing--;
    pthread_cond_broadcast(&non_write);
    pthread_mutex_unlock(&escri);
    //sem_post(&amarillito);
  }
  return NULL;
}

void * lector(void *arg){

  int v, i;
  int num = arg - (void*)0;
  while (1) {

    sleep(random() % 3);
    reader_lock();
    
    printf("Lector %d\n",num);
    v = arr[0];
    
    for (i = 1; i < ARRLEN; i++) {
      if (arr[i] != v)
        break;
    }
    printf("\tREADING:%d\n",reading);
    if (i < ARRLEN)
      printf("Lector %d, error de lectura\n", num);
    else
      printf("Lector %d, dato %d\n", num, v);
    
    printf("Dejo lector %d\n",num);

    reader_unlock();
    
  }

  return NULL;
}

int main(){

  pthread_t lectores[M], escritores[N];
  sem_init(&amarillito,0,1);
  int i;
  for (i = 0; i < M; i++){
    //pthread_mutex_init(&reader[i],NULL);
    pthread_create(&lectores[i], NULL, lector, i + (void*)0);
  }
  for (i = 0; i < N; i++)
    pthread_create(&escritores[i], NULL, escritor, i + (void*)0);
  pthread_join(lectores[0], NULL); /* Espera para siempre */
  return 0;
}