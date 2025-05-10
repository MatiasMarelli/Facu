#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>


typedef struct channel_{
  void* data;
  sem_t sem_reader,sem_writer;
  pthread_mutex_t mutex;
}channel;

void channel_init(channel* chan){
  sem_init(&(chan->sem_writer),0,0);
  sem_init(&(chan->sem_reader),0,0);
  pthread_mutex_init(&(chan->mutex),NULL);
  chan->data =NULL;
}

void chan_write(channel* chan, int v){
  sem_wait(&(chan->sem_writer));
  pthread_mutex_lock(&(chan->mutex));
  chan->data = (void*)&v;
  pthread_mutex_unlock(&(chan->mutex));
  sem_post(&(chan->sem_reader));
}

int chan_read(channel* chan){
  sem_post(&(chan->sem_writer));
  sem_wait(&(chan->sem_reader));
  return *(int*)(chan->data);
}

