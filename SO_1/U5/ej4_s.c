#include <stdlib.h>
#include <stdio.h>
#include "timing.h"
#include <math.h>


int isit=1;


void is_cousin(long double n){
  long double max_num =sqrt(n);
  for(long double i=2;i<max_num && isit;i++){
    if(modfl==0){
      isit=0;
    }
  }
}

int main(){
  long double n;
  /* printf("Prime:");
  scanf("%ld",&n); */
  n=powl(2,61)-1;
  
  
  printf("%Lf\n",n);
  float time;
  TIME_void(is_cousin(n),&time);

  if(isit){

    printf("Es primo");
  }else
    printf("no es ");
  return 0;
}