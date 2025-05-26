#include <omp.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "timing.h"


long double n;
int es_primo=1;

int is_div(long x, long n){
  return ((n % x) == 0);
}

void busqueda_primo(int id){
  printf("Hola soy:%d\n",id);
  long max_num =sqrt(n);
  long idx= (id*1.0)*(max_num/6);
  printf("Max_num:%ld y idx:%ld\n",max_num,idx);
  long i=0;
  if(idx==0)
  i=2;
  for(;i<max_num/12;i++){
    if(is_div((i+idx),n)){
      printf("Encontre divisor:%ld ,soy %d\n",i+idx,id);
      es_primo=0;
    }
  }
}


void fun(long n){
  long max_num =sqrt(n);
  printf("Srqt(n) = %ld\n",max_num);
  #pragma omp parallel num_threads(6)
  {
    #pragma omp for
    for(long i=2;i<max_num;i++){
      if(n%i==0){
        printf("Encontre divisor: %ld\n",i);
        es_primo=0;
      }
    }
  }
}

int main(){
 /*  printf("ELIJA UN NUMERO:");
  scanf("%ld",&n);
  printf("El numero:%ld\n",n); */
  n=powl(2,61)-1;
  printf("%Lf\n",n);
  float time;
  TIME_void(fun(n),&time);
  printf("Hola si,termine al final era...\n");
  if(es_primo){
    printf("Primo\n");
  }else{
    printf("No era primo\n");
  }

  return 0;
}