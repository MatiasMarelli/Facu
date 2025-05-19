#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <semaphore.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>



//void barrier_init(struct barrier *b, int n);
//void barrier_wait(struct barrier *b);

typedef struct barrier_{
  int n;
  volatile int count,count_sync;
  sem_t sem_sync;
  pthread_cond_t condition;
  pthread_mutex_t mut;
}barrier;

void barrier_init(barrier* bar,int num){
  bar->n=num;
  bar->count=0;
  bar->count_sync=0;
  sem_init(&(bar->sem_sync),0,0);
  pthread_mutex_init(&(bar->mut),NULL);
}

void barrier_wait2(barrier* bar){
  pthread_mutex_lock(&(bar->mut));
  
  bar->count++;
  if(bar->count == bar->n){
    bar->count=0;
    pthread_cond_broadcast(&(bar->condition));
    pthread_mutex_unlock(&(bar->mut));
    return ;
  }else{
    while(bar->count != 0){
      pthread_cond_wait(&(bar->condition),&(bar->mut));
    }
    pthread_mutex_unlock(&(bar->mut));

  }
  return;
}



void barrier_wait(barrier* bar){
  while(bar->count_sync != 0);
  pthread_mutex_lock(&(bar->mut));

  bar->count++;

  if(bar->count == bar->n){
    bar->count=0;
    pthread_cond_broadcast(&(bar->condition));

    bar->count_sync = bar->n-1;
    pthread_mutex_unlock(&(bar->mut));
    
    sem_wait(&bar->sem_sync);
    
  }else{
    while(bar->count != 0){
      pthread_cond_wait(&(bar->condition),&(bar->mut));
    }
    bar->count_sync--; // empiezo sync
    if(bar->count_sync == 0) sem_post(&bar->sem_sync); // si ya somos 4, pasamos
    pthread_mutex_unlock(&(bar->mut));

  }
}

































#define N 100
#define W 12
#define ITER 10

float arr1[N], arr2[N];

pthread_barrier_t barr;
//barrier barr;

//void barrier_init(struct barrier *b, int n);
//void barrier_wait(struct barrier *b);

int min(int n, int m){
  return n<=m ? n : m;
}

void calor(float *arr, int lo, int hi, float *arr2) {
  int i;
  for (i = lo; i < hi; i++) {
    //printf("i: %d\n",i);
    int m = arr[i];
    int l = i > 0 ? arr[i-1] : m;
    int r = i < N-1 ? arr[i+1] : m;
    arr2[i] = m + (l - m)/1000.0 + (r - m)/1000.0;
  }
}

void print (){
  for(int i=0;i<N;i++){

    printf("%f ",arr1[i]);
  }
  printf("\n");
  for(int i=0;i<N;i++){
    printf("%f ",arr2[i]);
  }
  printf("\n");
  
}

/* Dado un array de [n], devuelve el punto de corte [i] de los [m] totales. */

static inline int cut(int n, int i, int m) {
  return i * (n/m) + min(i, n %m);
}
void * thr(void *arg) {
  int id = *(int*)arg; /* 0 <= id < W */
  int lo = cut(N, id, W), hi = cut(N, id+1, W);
  int i;

  for (i = 0; i < ITER; i++) {
    printf("ANTES CALOR 1,Hilo: %d\n",gettid());
    //print();
    calor(arr1, lo, hi, arr2);
    printf("DESPUES CALOR 1,Hilo: %d\n",gettid());
    //print();
    pthread_barrier_wait(&barr);
    //barrier_wait(&barr);
    calor(arr2, lo, hi, arr1);
    printf("\nDESPUES CALOR 2, Hilo:%d\n",gettid());
    pthread_barrier_wait(&barr);
    //barrier_wait(&barr);

    //print();
  }
  return NULL;
}

void promedio(){
  float sum1=0;
  float sum2=0;
  for (int i=0;i<N;i++){
    sum1 =+ arr1[i];
    sum2 =+ arr2[i];
  }
  printf("Promedio arr1:%f, Promedio arr2:%f \n",sum1/N,sum2/N);
}

int main(){
  srand(time(NULL));
  for(int i=0; i<N;i++){
    //arr1[i]= ((rand()%1000)*1.0)/((rand()%15*1.0)+1);
    //arr2[i]= ((rand() %1000)*1.0)/((rand()%15*1.0)+1);
    arr1[i]=i;
    arr2[i]=i;
  }

  pthread_t h [W];
  pthread_barrier_init(&barr,NULL,W);
  //barrier_init(&barr,W);


  for(int i=0 ;i<W;i++){
    pthread_create(&h[i],NULL,thr,(void*)&i);
  }

  for(int i=0;i<W;i++){
    pthread_join(h[i],NULL);
  }
  
  promedio();

  return 0;
}

/*print arr1, arr2
calor() (1) (toca arr2)
print arr1, arr2 - arr1 tiene que estar igual
calor() (2) (toca arr1)
print arr1, arr2 - arr2 tiene que estar igual */