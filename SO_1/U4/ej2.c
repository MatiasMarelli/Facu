#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N_FILOSOFOS 5
#define ESPERA 500000
sem_t sematoto;

pthread_mutex_t tenedor [N_FILOSOFOS];

pthread_mutex_t * izq(int i) { return &tenedor[i]; }
pthread_mutex_t * der(int i) { return &tenedor[(i+1) % N_FILOSOFOS]; }

void pensar(int i){

  printf("Filosofo %d pensando...\n", i);
  usleep(random() % ESPERA);

}
void comer(int i){

  printf("Filosofo %d comiendo...\n", i);
  usleep(random() % ESPERA);

}
void tomar_tenedores(int i){
  //sem_wait(&sematoto);
  int a =1;
  while(a){

    pthread_mutex_lock(der(i));
    if(pthread_mutex_trylock(izq(i)) != 0){
      printf("UF\n");
      pthread_mutex_unlock(der(i));
      usleep(random() % ESPERA);
    }else{
      a=0;
    }
  }
  

}

void tomar_zurdo(int i){
  pthread_mutex_lock(izq(i));
  pthread_mutex_lock(der(i)); 
}
void dejar_tenedores(int i){
  
  pthread_mutex_unlock(der(i));
  pthread_mutex_unlock(izq(i));
  //sem_post(&sematoto);

}
void * filosofo(void *arg){
  int i = arg - (void*)0;
  while (1) {


    tomar_tenedores(i);
    comer(i);
    dejar_tenedores(i);
    pensar(i);


    /* if(i==4){
      tomar_zurdo(i);
      comer(i);
      dejar_tenedores(i);
      pensar(i);
    }else{
      tomar_tenedores(i);
      comer(i);
      dejar_tenedores(i);
      pensar(i);
    } */
  }
}
int main(){
  sem_init(&sematoto,0,4);

  pthread_t filo[N_FILOSOFOS];
  int i;
  for (i = 0; i < N_FILOSOFOS; i++)
    pthread_mutex_init(&tenedor[i], NULL);
  for (i = 0; i < N_FILOSOFOS; i++)
    pthread_create(&filo[i], NULL, filosofo, i + (void*)0);
  pthread_join(filo[0], NULL);
  
  sem_destroy(&sematoto);
  return 0;
}