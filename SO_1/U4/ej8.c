#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <semaphore.h>



//void barrier_init(struct barrier *b, int n);
//void barrier_wait(struct barrier *b);

typedef struct barrier_{
  int n,count;
  //sem_t sem;
  pthread_cond_t condition;
  pthread_mutex_t mut;
}barrier;

void barrier_init(barrier* bar,int num){
  bar->n=num;
  bar->count=0;
  pthread_mutex_init(&(bar->mut),NULL);
}

void barrier_wait(barrier* bar){
  pthread_mutex_lock(&(bar->mut));
  if(count == bar->n){
    pthread_mutex_unlock(&(bar->mut));
    return ;
  }
  bar->count++;
  if(count == bar->n){
    pthread_cond_broadcast(&(bar->condition));
    pthread_mutex_unlock(&(bar->mut));
    return ;
  }
  while(bar->count != bar->n){
    pthread_cond_wait(&(bar->condition),&(bar->mut));
  }
  pthread_mutex_unlock(&(bar->mut));
  return;
}





































#define N 20
#define W 7
#define ITER 10

float arr1[N], arr2[N];

//pthread_barrier_t barr;

//void barrier_init(struct barrier *b, int n);
//void barrier_wait(struct barrier *b);

int min(int n, int m){
  return n>=m ? n : m;
}

void calor(float *arr, int lo, int hi, float *arr2) {
  int i;
  for (i = lo; i < hi; i++) {
    int m = arr[i];
    int l = i > 0 ? arr[i-1] : m;
    int r = i < N-1 ? arr[i+1] : m;
    arr2[i] = m + (l - m)/1000.0 + (r - m)/1000.0;
  }
}
/* Dado un array de [n], devuelve el punto de corte [i] de los [m] totales. */
static inline int cut(int n, int i, int m) {
  return i * (n/m) + min(i, n %m);
}
void * thr(void *arg) {
  int id = arg - (void*)0; /* 0 <= id < W */
  int lo = cut(N, id, W), hi = cut(N, id+1, W);
  int i;

  for (i = 0; i < ITER; i++) {
    calor(arr1, lo, hi, arr2);
    //pthread_barrier_wait(&barr);
    calor(arr2, lo, hi, arr1);
  }
  return NULL;
}

int main(){

  pthread_t h [ITER];
  //pthread_barrier_init(&barr,NULL,ITER);
  for(int i=0 ;i<ITER;i++){
    pthread_create(&h[i],NULL,thr,(void*)&i);
  }

  for(int i=0;i<ITER;i++){
    pthread_join(h[i],NULL);
  }
  
  for(int i=0;i<N;i++){

    printf("%f ",arr1[i]);
  }
  printf("\n");
  for(int i=0;i<N;i++){
    printf("%f ",arr2[i]);
  }
  printf("\n");

  return 0;
}
