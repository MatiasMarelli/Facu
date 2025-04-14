#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>

int i=0;
int result=0;

int* make_array(int n){
  int* a=malloc(sizeof(int)*n);
  for(int j=1;j <= n; j++){
    a[j-1]=j;
  }
  return a;
}

void* sum_arr(void** arg){
  int n=*(int*)arg[1];
  int* a=(int*)arg[0];
  for(;i<n;i++){
    result += a[i];
  }
  
}

int sum(int n){return (n*(n+1)/2);}

int main(){
  int* n;
  srand(time(NULL));
  *n=rand()%7483647;
  printf("\n\t %d \t",*n);
  int* arr;
  arr=make_array(*n);
  void** args =malloc(sizeof(void*)*2);
  args[0]=(void*)arr;
  args[1]=(void*)n;

  pthread_t h1,h2;
  pthread_create(&h1,NULL,sum_arr,args);
  pthread_create(&h2,NULL,sum_arr,args);
  
  
  pthread_join(h1,NULL);
  pthread_join(h2,NULL);
  
  printf("Result sum thread: %d\n",result);  
  printf("Suma Gauss: %d",sum(*n));

  return 0;
}