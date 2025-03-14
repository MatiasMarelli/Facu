#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int main(){
  int a;
  char* token;

  while(1){
    char* buffer = malloc(sizeof(char)*255);
    fgets(buffer,255,stdin);
    buffer[strlen(buffer) - 1] = '\0';
    if(!strcmp(buffer,"exit")){exit(0);}
    
    pid_t pid=fork();
    if(pid==0){
      int i=0;
      char* args[255];
      token=strtok(buffer," ");
      while(token!=NULL){
        args[i]=token;
        token=strtok(NULL," ");
        i++;
      }
      args[i]=NULL;
      if(execv(args[0],args) == -1){kill(getpid(),SIGKILL);}
    } else {
      wait(&a);
    }
    free(buffer);
  }
  return 0;
}