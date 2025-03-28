#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


void exe(char** args){
  if(execv(args[0],args) == -1){kill(getpid(),SIGKILL);}
}


int main(){
  int a;
  char* token;
  //int std=dup(STDOUT_FILENO); 
  while(1){
    char* buffer = malloc(sizeof(char)*255);
    fgets(buffer,255,stdin);
    buffer[strlen(buffer) - 1] = '\0';
    if(!strcmp(buffer,"exit")){exit(0);}
    //dup2(1,STDOUT_FILENO);


    pid_t pid=fork();
    if(pid==0){
      int i=0;
      char* args[255];
      token=strtok(buffer," ");
      
      while(token!=NULL){
        args[i]=token;
        token=strtok(NULL," ");
        i++;
        //if(!strcmp(">",token)) flag=0;
      }

      args[i]=NULL;
      printf("%d\n",i);
      // printf("%s\n",args[i-2]);
      for(int j=0;j<=i;j++){
        printf("%d:%s\n",j,args[j]);
      }
      if(i>=2 && !strcmp(args[i-2],">")){
        printf("\n > \n");
        //token=strtok(NULL," ");
        FILE* f= fopen(args[i-1],"w");
        //close(1);
        int fd = fileno(f);
        dup2(fd,1);
        args[i-2]=NULL;
        exe(args);
      }else{
        exe(args);
      }
      //if(flag) exe(args);

      //if(!flag){
      //  printf("\n > \n");
      //  token=strtok(NULL," ");
      //  FILE* f= fopen(token,"w");
      //  //close(1);
      //  int fd = fileno(f);
      //  dup2(fd,1);
      //  //close(STDOUT_FILENO);
      //}
      //sleep(1);
      

    }else {
      wait(&a);
      
    }
    free(buffer);
  }


  return 0;
}