#include <omp.h>
#include <stdio.h>

int main(){
  int i = 10;
  #pragma omp parallel private(i)
  {
    printf("Thrad: %d, i =%d\n",omp_get_thread_num(),i);
    i=1000;
  }
  printf("i=%d\n",i);

  return 0;
}