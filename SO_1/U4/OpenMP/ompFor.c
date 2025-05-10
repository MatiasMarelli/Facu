#include <stdio.h>
#include <omp.h>
#include <unistd.h>


int main(){
  int ans=0;
  #pragma omp parallel
  {
    #pragma omp for
    for(int i = 0;i<100;i++){
      ans = i;
      printf("id: %d ans: %d\n",omp_get_thread_num(),ans);
      sleep(2);
    }

  }


  return 0;
}