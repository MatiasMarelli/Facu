#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>



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
      open("TEMP.txt",'r');
      int fd;
      int i=0;
      int flag=0;
      char* args[255];
      token=strtok(buffer," ");
      
      while(token!=NULL){
        args[i]=token;
        token=strtok(NULL," ");
        i++;
        //if(!strcmp(">",token)) flag=0;
        if(!strcmp(args[i-1],"|") || token == NULL){
          printf("\n\t | \n");
          int pipeline[2];
          pipe(pipeline);
          pid_t nieto = fork();
          if(nieto == 0){
            close(pipeline[0]);
            if(flag == 0){
              if(token==NULL){
                printf("CASO 0\n");
                args[i]=NULL;
              }else{
                printf("CASO 0\n");
                args[i-1]=NULL;
              }
              for(int j=0;j<i;j++){
                printf("j:%d, argv:%s\n",j,args[j]);
              }
            }else{
              printf("CASO 1\n");
              printf("openat: %d\n",openat(fd,"TEMP.txt",'r'));
              strcpy(args[i++],"TEMP");  //fdopen(fd,"r");
              args[i++] = NULL;
              for(int j=0;j<i;j++){
                printf("j:%d, argv:%s\n",j,args[j]);
              }
            }
            //if(token != NULL) dup2(pipeline[1],1);
            dup2(pipeline[1],1);
            exe(args);
            
          }else{
            char buff[255];
            wait(NULL);
            close(pipeline[1]);
            fd=dup(pipeline[0]);
            read(pipeline[0],buff,255);
            printf("BUFFER:%s\n",buff);
            
            //close(pipeline[0]);
          }
          flag++;
         
          i=0;
        }
      }
      printf("END WHILE\n");
     /*  if(flag==0){
        args[i]=NULL;
      }else{
        args[i]=fdopen(fd,"r");
        args[i+1]=NULL;
      } */
      //printf("%d\n",i);
      // printf("%s\n",args[i-2]);
      
      /* if(i>=2 && !strcmp(args[i-2],">")){
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
      } */

      
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