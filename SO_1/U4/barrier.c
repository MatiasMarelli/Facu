#include <pthread.h>
#include <stdio.h>



pthread_barrier_t barrier;

void* thread_function(void* arg){
  printf("Thread %d antes de barrier\n",*(int*) arg);

  pthread_barrier_wait(&barrier);

  printf("Thread %d post barrier\n",*(int*)arg);

  return NULL;

}


int main(){

  pthread_barrier_init(&barrier,NULL,3);

  pthread_t thread[3];

  for(int i=0;i<3;i++){
    printf("Crear thread %d\n",i);
    pthread_create(&thread[i],NULL,thread_function,(void*) &i);
  }

  pthread_join(thread[0],NULL);
  pthread_join(thread[1],NULL);
  pthread_join(thread[2],NULL);
  return 0;
}