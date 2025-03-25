#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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


int main(){  
  e();
  return 0;
}