#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>


#define NUM_VISITANTE 40000000

int visitante=0;
//int flag=0; //0 Esta desocupada, 1 esta ocupada;

int turno=0; // 0, todavia no se asigno, 1 =>molinete 1 | 2 => molinete 2

int flags[2] = {0,0}; // 0 => no tengo intencion de entrar | 1 => Tengo intencion de entrar

pthread_mutex_t mutex_visitante = PTHREAD_MUTEX_INITIALIZER;


void* molinete(void* arg){

  for(int i=0;i<NUM_VISITANTE/2;i++){
   /*  //  LOCK
    while(flag);
    flag=1;

    //ZONA CRITICA
    visitante ++;
    //  UNLOCK
    flag=0; 
    */
    /* 
    // LOCK
    turno=2;

    while(turno==2);
    // ZONA CRITICA
    visitante++;

    //UNLOCK
    turno=2; 
    */


    // ESTE ANDA COMO EL ORTO
    /* 

    flags[0] = 1; // Molinete 1, tiene intencion de entrar.

    while(flags[1]==1){;}

    visitante ++;

    flags[0]=0; 
    */
    /* 
    flags[0]=1;
    turno=2;
    while(flags[1] == 1 && turno == 2){;}

    visitante ++;

    flags[0]=0; 
    
    */
    
    pthread_mutex_lock(&mutex_visitante);
    visitante++;
    pthread_mutex_unlock(&mutex_visitante);

  }

  return NULL;
}

void* molinete2(void* arg){
  for(int i=0;i<NUM_VISITANTE/2;i++){
    /* //  LOCK
    while(flag);
    flag=1;

    //ZONA CRITICA
    visitante ++;
    //  UNLOCK
    flag=0; */

    /* turno=1;

    while(turno==1);

    visitante++;

    turno=1;
    */
    //ESTE ANDA COMO EL ORTO
    /* 
    flags[1]=1;
    while(flags[0]==1){;}

    visitante ++;

    flags[1]=0;
   */

   /*  
    flags[1]=1;
    turno=1;
    while(flags[0] == 1 && turno == 1){;}

    visitante ++;

    flags[1]=0; 
    */


    pthread_mutex_lock(&mutex_visitante);
    visitante++;
    pthread_mutex_unlock(&mutex_visitante);


  }

  return NULL;
}

int main(){
    pthread_t h1,h2;
    pthread_create(&h1,NULL,molinete,NULL);
    pthread_create(&h2,NULL,molinete2,NULL);
    
    //printf("Main joinning...\n");

    pthread_join(h1,NULL);
    pthread_join(h2,NULL);

    pthread_mutex_destroy(&mutex_visitante);

  printf("Cantidad visitantes: %d\n",visitante);

  return 0;
}