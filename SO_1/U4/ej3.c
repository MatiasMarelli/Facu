#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define M 5
#define N 5
#define SZ 2
int contador = 0;
pthread_mutex_t mutex_prod = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cons = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cprod, ccons;

sem_t semtoto_prod, semtoto_cons;

/*
 * El buffer guarda punteros a enteros, los
 * productores los consiguen con malloc() y los
 * consumidores los liberan con free()
 */
int *buffer[SZ];

void enviar(int *p)
{

  sem_wait(&semtoto_prod);
  pthread_mutex_lock(&mutex_prod);

	while(contador+1 == SZ){
    //pthread_cond_wait(&cprod,&mutex_prod);
  }
  buffer[contador]=p;
  contador++;
  //pthread_cond_signal(&ccons);

  pthread_mutex_unlock(&mutex_prod);
  sem_post(&semtoto_prod);
	return;
}

int * recibir()
{

  int* p;
  sem_wait(&semtoto_cons);
	pthread_mutex_lock(&mutex_cons);

	while(contador == 0){
    //pthread_cond_wait(&ccons,&mutex_cons);
  }

  contador--;
  p=buffer[contador];
  //pthread_cond_signal(&cprod);

  pthread_mutex_unlock(&mutex_cons);
	sem_post(&semtoto_cons);
	return p;
}

void * prod_f(void *arg)
{
	int id = arg - (void*)0;
	while (1) {
		sleep(random() % 2);

		int *p = malloc(sizeof *p);
		*p = random() % 100;
		printf("Productor %d: produje %p->%d\n", id, p, *p);
		enviar(p);
	}
	return NULL;
}

void * cons_f(void *arg)
{
	int id = arg - (void*)0;
	while (1) {
		sleep(random() % 2);

		int *p = recibir();
		printf("Consumidor %d: obtuve %p->%d\n", id, p, *p);
		free(p);
	}
	return NULL;
}

int main()
{
  sem_init(&semtoto_prod,0,1);
  sem_init(&semtoto_cons,0,1);
	pthread_t productores[M], consumidores[N];
	int i;

	for (i = 0; i < M; i++)
		pthread_create(&productores[i], NULL, prod_f, i + (void*)0);

	for (i = 0; i < N; i++)
		pthread_create(&consumidores[i], NULL, cons_f, i + (void*)0);

	pthread_join(productores[0], NULL); /* Espera para siempre */
  sem_destroy(&semtoto_prod);
  sem_destroy(&semtoto_cons);
	return 0;
}