#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>

#define N 1000000

int visitantes=0;

void * proc(void *arg) {
  int i;
  int id = arg - (void*)0;
  for (i = 0; i < N/2; i++) {
    int c;
    /* sleep? */
    c = visitantes;
    sleep(0.1);
    /* sleep? */
    visitantes = c + 1;
    /* sleep? */
  }
  return NULL;
}

void * proc2(void *arg) {
  int i;
  int id = arg - (void*)0;
  for (i = 0; i < N/2; i++) {
    int c;
    /* sleep? */
    c = visitantes;
    sleep(0.1);
    /* sleep? */
    visitantes = c + 1;
    /* sleep? */
  }
  return NULL;
}

int main(){
  pthread_t h1,h2;
  pthread_create(&h1,NULL,proc,NULL);
  pthread_create(&h2,NULL,proc2,NULL);
    
    //printf("Main joinning...\n");
  pthread_join(h1,NULL);
  pthread_join(h2,NULL);

  printf("%d\n",visitantes);

  return 0;

}