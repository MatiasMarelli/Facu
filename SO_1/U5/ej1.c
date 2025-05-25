#include <omp.h>
#include <stdio.h>
#include <time.h>

#define N 12
#define X 100000

int arrNums[N] = {0};
int arrEligiendo[N] = {0};
int num=0;;

int maxArr(int *arr, int len) {
  int res = arr[0];
  for(int i = 0; i < len; i++) {
    if(res < arr[i]) {
      res = arr[i];
    }
  }
  return res;
}


void fun(int indice) {
  //int indice = *(int*) ptr;
  arrEligiendo[indice] = 1;
  #pragma omp critical
  {
    arrNums[indice] = 1 + maxArr(arrNums,N);
    arrEligiendo[indice] = 0;
  }
  /*
  Si el hilo j está calculando su número, espera a que termine
  Si el hilo j tiene más prioridad, espera a que ponga su número a cero 
  j tiene más prioridad si su número de turno es más bajo que el de i
  o bien si es el mismo número y además j es menor que i
  */
  for (int j = 0; j < N; j++) {
    while ( (arrEligiendo[j] == 1) || (arrNums[j] != 0 && 
        ((arrNums[j] < arrNums[indice]) || 
          ((arrNums[j] == arrNums[indice]) && (j < indice))))); // Nada
  }
  
  // Inicio Seccion critica
  #pragma omp critical 
  {
    for (int i = 0; i < X; i++) {
      num++;
    }
  }
  // Fin seccion critica
  arrNums[indice] = 0;
}


int main(){
  clock_t start, end;
  double cpu_time_used;

  start = clock();
  
  
  #pragma omp parallel num_threads(N)
  {
    int id =omp_get_thread_num();
    fun(id);
    //#pragma omp barrier
  }

  end = clock();
  
  printf("Entraron %d\n", num);
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("Tiempo de CPU usado: %f segundos\n", cpu_time_used);
}