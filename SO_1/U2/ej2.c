#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void a(){
  char buffer[255];
  close(STDIN_FILENO);
  write(STDIN_FILENO,"HOLAARX",8);
}

void b (){
  close(STDOUT_FILENO);
  printf("Aca deberia printear algo...");
}

void c(){
  int duplicate =dup(STDIN_FILENO);
  char buffer[]="HOLAAA";
  close(STDIN_FILENO);
  write(duplicate,buffer,strlen(buffer));
  //write(STDIN_FILENO,buffer,strlen(buffer));

}

void d(){
  pid_t pid=fork();
  if(pid==0){
    printf("pid hijo:%d\n",getpid());
  } else{
    printf("pid padre:%d\n",getpid());
  }
} 

void e(){
  pid_t pid=fork();
  if(pid==0) {printf("%d\n",getpid());exit(0);}
  else{
    while(1);
  }
}

void f (){
  int *a=malloc(sizeof(char)*2^50);
  //La memoria no cambia (supongo q el sistema operativo no debe ceder la memoria salvo que sea utilizada y/o se empiece a llenar)
  printf("\nPid:%d\n",getpid());
  while(1);
}

void g (){
  pid_t pid=fork();
  if(!pid){
    printf("\nPid:%d\n",getpid());
    printf("sleeping...\n");
    sleep(10); 
    while(1);
    execl("./test","./test",NULL);
  }else{
    wait(0);
    printf("Termino Hijo\n");
  }
}

int main(){  
  g();
  return 0;
}