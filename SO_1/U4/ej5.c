#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define SILLAS 5

int cant_clientes=0;
sem_t rojito;
pthread_mutex_t barba=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t atendido=PTHREAD_MUTEX_INITIALIZER;
int n;
int awake=0;

pthread_mutex_t* mutex_cli;
pthread_cond_t b,c;

void me_cortan(){
  sleep(2);
}
void cortando(){
  sleep(2);
}

void pagando(int n){
  printf("Me cortaron id:%d\n",n);
}
void me_pagan(){
  printf("Corte\n");
}

void irse(int id){
  printf("Me fui... %d\n",id);
  sleep(random() % 5);
}

void* el_tijeras(void* arg){
  while(1){
    awake=0;
    pthread_cond_signal(&c);
    pthread_mutex_lock(&barba);
    while(cant_clientes==0){
      printf("\nMe dormi...\n");
      pthread_cond_wait(&b,&barba);
    }
    awake=1;  
    printf("\tCLIENTES:%d\n",cant_clientes);
    //sleep(1);
    cortando();    
    
    me_pagan();
    pthread_mutex_unlock(&barba);
  } 

  return NULL;
}

void* cliente(void* arg){
  int id =*(int*)arg;
  while(1){
    sleep(random() % 5);
    int me_quiero_cortar=1;

    pthread_mutex_lock(&mutex_cli[id]);
    while(me_quiero_cortar){
      if(sem_trywait(&rojito)!=0){
        irse(id);
      }else me_quiero_cortar =0;
    }
    sleep(3);
    cant_clientes++;
    pthread_mutex_unlock(&mutex_cli[id]);

    pthread_mutex_lock(&atendido);
    pthread_cond_signal(&b);

    while(!awake){
      pthread_cond_wait(&c,&atendido);
    }
    
    me_cortan();
    
    pagando(id);
    cant_clientes--;
    
    pthread_cond_signal(&b);
    sem_post(&rojito);
    pthread_mutex_unlock(&atendido);
  }
  return NULL;
}



int main(){
  n=(random() % 10)+SILLAS;
  mutex_cli = malloc(sizeof(pthread_mutex_t)*n);
  pthread_t barbero,clientes[n];
  sem_init(&rojito,0,SILLAS);


  pthread_create(&barbero,NULL,el_tijeras,NULL);
  
  for(int i=0;i<n;i++) {
    pthread_mutex_init(&mutex_cli[i],NULL);
    pthread_create(&clientes[i],NULL,cliente,(void*)&i);
  }

  pthread_join(barbero,NULL);
  for(int i=0;i<n;i++) pthread_mutex_destroy(&mutex_cli[i]);
  free(mutex_cli);
  sem_destroy(&rojito);
  return 0;
}