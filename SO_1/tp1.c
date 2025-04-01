
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>

int* shuffle(int n){ //Funcion que desordena aleatoriamente una lista que va desde [1...n]
  int * l=malloc(sizeof(int)*n);
  for(int i=0;i<n;i++){l[i]=i+1;}
  for(int i=0;i<n;i++){
    int a=rand() % n;
    int temp=l[i];
    l[i]=l[a];
    l[a]=temp;
  }
  return l;
}
int main(){
  int n=0;
  scanf("%d",&n); //Tomamos n por la entrada estandar.
  
  
  srand(time(NULL)); //Ajustamos la seed para que sea aleatoria la secuancia
  
  int *l = shuffle(n);
  
  int* dir= l; //Guardamos la direccion original de la lista, para luego liberarla.
  
  /* for(int i=0;i<n;i++){printf("%d ",l[i]);}
  printf("\n"); */

  while(n>=3){
    int fd[2];
    pipe(fd); //Pipeline
    pid_t pid =fork();
    if(!pid){ // Proceso Child
      close(fd[0]);
      int buff[3];
      buff[0]=l[0];
      buff[1]=l[1];
      buff[2]=l[2];
      write(fd[1],buff,12); //El hijo escribe
      exit(0);
    }else{ //Proceso Parent
      wait(NULL);
      close(fd[1]); //El padre lee
      int r[3];
      read(fd[0],r,12);
      printf("%d %d %d\n",r[0],r[1],r[2]); 
    }
    l=l+3;
    n=n-3;
  }
  if(n!=0){ //Casos donde n no es multiplo de 3!
    n==1 ? printf("%d\n",l[0]) : printf("%d %d \n",l[0],l[1]);
  }

  free(dir); //Liberamos el array de enteros.
  return 0;
}