#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int main(){
  int a;
  //pid_t pid=fork();
  const char s[2]=" ";
  char* token;
  char* buffer = malloc(sizeof(char)*30);
  while(1){
  pid_t pid=fork();
  
  if(pid==0){
      fgets(buffer,30,stdin);
      printf("Buffer:%s\n Token:%s\n",buffer,token);
      if(!strcmp(buffer,"exit")){exit(0);}
      int i=0;
      char*args[255];
      token=strtok(buffer,s);
      while(token!=NULL){
        args[i]=token;
        token=strtok(NULL,s);
        i++; 
      }
      args[i]=NULL;
      //scanf("%[^\n]",buffer);
      //fd=open("shell.txt",'w');
      //write(fd,buffer,100);
      
      
      //bailalo ROCKY : )
      execv(args[0],args);
    }
    else{
      wait(&a);
      //printf("\nclear buffer\n");
    }
    
  }
  free(buffer);
  return 0;
}