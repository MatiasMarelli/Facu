#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>



int main(){
  
  int fd[2];
  char buffer[]= "Rax -> Arx :O ";
  char readbuffer;
  pipe(fd);

  pid_t pid=fork();

  if(pid == 0){


    close(fd[1]);
    int flag=1;
    while(flag!=0){

      flag=read(fd[0],&readbuffer,1);

      printf("%c",readbuffer);
    }
    close(fd[0]);

  }else{


    close(fd[0]);

    write(fd[1],buffer,strlen(buffer));
    close(fd[1]);
  }




}