#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>

#define N 1000
#define W 4
#define ITERS 5

//void barrier_init(struct barrier *b, int n);
//void barrier_wait(struct barrier *b);

typedef struct barrier_{
  int n;
  volatile int fst,count,count_sync;
  pthread_spinlock_t lock,loop;
}barrier;

void aux(barrier*);

void barrier_init(barrier* bar,int num){
  bar->n=num;
  bar->count=0;
  bar->fst=1;
  bar->count_sync=0;
  pthread_spin_init(&(bar->lock),PTHREAD_PROCESS_SHARED);
  pthread_spin_init(&(bar->loop),PTHREAD_PROCESS_SHARED);
 
}



void aux_lock(void* b){
  barrier* bar=(barrier*) b;
  pthread_spin_lock(&(bar->loop));
  printf("Se creo el AUX\n");
  int a=1;
  pthread_spin_unlock(&bar->lock);
  while(a){

    printf("\nEN EL WHILE\n");
    usleep(1000);

    pthread_spin_lock(&bar->lock);
    if(bar->count == bar->n -1 ){
      a=0;
    }else{
      pthread_spin_unlock(&bar->lock);
    }

  }
  printf("SALI\n");

  //pthread_spin_lock(&bar->lock);

  bar->count=0;
  bar->fst=1;
  bar->count_sync = (bar->n)-1;
  pthread_spin_unlock(&bar->lock);
  pthread_spin_unlock(&(bar->loop));
  return ;
}




void barrier_wait(barrier* bar){
  
  while(bar->count_sync != 0);
  pthread_spin_lock(&(bar->lock));
  if(bar->fst==1){
    bar->fst=0;
    aux_lock(bar);
    return ;
  }

  bar->count++;
  printf("COUNT:%d\n",bar->count);
    
  
  pthread_spin_unlock(&bar->lock);
  while(bar->count != 0){
    //printf("Entre al while %d\n",bar->count);
    pthread_spin_lock(&bar->loop);
  }
  pthread_spin_unlock(&bar->loop);
  pthread_spin_lock(&bar->lock);
  bar->count_sync--; // empiezo sync
  
  pthread_spin_unlock(&bar->lock);
  return;  
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

	for (i = 0; i < ITERS; i++) {                 //PRINTS PARA TESTEAR LOS DEADLOCKS DENTRO DE LA FUNCIONALIDAD DE LAS BARRIERS
		//printf("ANTES CALOR 1,Hilo: %d\n",gettid());

    calor(arr1, lo, hi, arr2);
    //printf("DESPUES CALOR 1,Hilo: %d\n",gettid());

    barrier_wait(&barr);
    calor(arr2, lo, hi, arr1);
    //printf("\nDESPUES CALOR 2, Hilo:%d\n",gettid());
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
		f += arr1[i];
	f /= N;

	/* Imprimimos temperatura promedio, siempre debería dar igual */
	printf("Temp promedio: %f\n", f);
	return 0;
}