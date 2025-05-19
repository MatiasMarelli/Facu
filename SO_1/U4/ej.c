#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 1000
#define W 4
#define ITERS 10000





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




















float arr1[N];
float arr2[N];
barrier barr;

static inline int min(int x, int y) { return x < y ? x : y; }

void calor(float *in, int lo, int hi, float *out)
{
	int i;
	for (i = lo; i < hi; i++) {
		int m = in[i];
		int l = i > 0   ? in[i-1] : m;
		int r = i < N-1 ? in[i+1] : m;
		out[i] = m + (l - m)/1000.0 + (r - m)/1000.0;
	}
}

/* Dado un array de [n], devuelve el punto de corte [i] de los [m] totales. */
static inline int cut(int n, int i, int m)
{
	return i * (n/m) + min(i, n%m);
}

void * thr(void *arg)
{
	int id = arg - (void*)0;
	int lo = cut(N, id, W);
	int hi = cut(N, id+1, W);
	int i;

	for (i = 0; i < ITERS; i++) {
		calor(arr1, lo, hi, arr2);
    barrier_wait(&barr);
		calor(arr2, lo, hi, arr1);
    barrier_wait(&barr);
	}
}

int main()
{
	pthread_t w[W];
	float f;
	int i;
  barrier_init(&barr,W);
	/*
	 * No cambiamos la semilla, por lo que este programa
	 * debería ser determinista
	 */
	for (i = 0; i < N; i++)
		arr1[i] = rand() / 10000.0;

	for (i = 0; i < W; i++)
		pthread_create(&w[i], NULL, thr, i + (void*)0);

	for (i = 0; i < W; i++)
		pthread_join(w[i], NULL);

	f = 0;
	for (i = 0; i < N; i++)
		f += arr2[i];
	f /= N;

	/* Imprimimos temperatura promedio, siempre debería dar igual */
	printf("Temp promedio: %f\n", f);
	return 0;
}