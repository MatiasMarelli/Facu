#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 1000
#define W 4
#define ITERS 5

float arr1[N];
float arr2[N];

struct barrier {
  pthread_cond_t cv, in;
  pthread_mutex_t lock;
  sem_t sem;
  int n, waiting, exit;
};

struct barrier barr;

void barrier_init(struct barrier* barr, int n){
  pthread_cond_init(&barr->cv,NULL);
  sem_init(&barr->sem,0,0);
  pthread_mutex_init(&barr->lock,NULL);
  barr->n = n;
  barr->waiting = 0;
  barr->exit = 0;
}

void barrier_wait(struct barrier* barr){
  pthread_mutex_lock(&barr->lock);
  while(barr->exit != 0){
    pthread_cond_wait(&barr->in,&barr->lock);
  }
  pthread_mutex_unlock(&barr->lock);

  pthread_mutex_lock(&barr->lock);
  barr->waiting++;
  if(barr->waiting < barr->n){
    while(barr->waiting < barr->n){
      pthread_cond_wait(&barr->cv,&barr->lock);
    }
    barr->exit--;
    if(barr->exit == 0){ 
      sem_post(&barr->sem);
      barr->waiting = 0;
    }
    pthread_mutex_unlock(&barr->lock);
  }
else{
    barr->exit = barr->n - 1;
    pthread_cond_broadcast(&barr->cv);
    pthread_mutex_unlock(&barr->lock);
    sem_wait(&barr->sem);
    pthread_cond_broadcast(&barr->in);
  }
}

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
    printf("ANTES CALOR 1,Hilo: %d\n",gettid());
		calor(arr1, lo, hi, arr2);
    printf("Desp CALOR 1,Hilo: %d\n",gettid());
    barrier_wait(&barr);
		calor(arr2, lo, hi, arr1);
    printf("Desp CALOR 2,Hilo: %d\n",gettid());
    barrier_wait(&barr);
	}
}

int main()
{
	pthread_t w[W];
	float f1;
	float f2;
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

	f1 = 0;
  f2 = 0;
	for (i = 0; i < N; i++){
		f1 += arr1[i];
    f2 += arr2[i];
  }
	f1 /= N;
	f2 /= N;

	/* Imprimimos temperatura promedio, siempre debería dar igual */
	printf("Temp promedio 1: %f\n", f1);
	printf("Temp promedio 2: %f\n", f2);
	return 0;
}