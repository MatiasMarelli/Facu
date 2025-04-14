#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>


void exe(char** args){
  if(execvp(args[0],args) == -1){kill(getpid(),SIGKILL);}
}

void redir(char** args,int index){
  FILE* f= fopen(args[index-1],"w");
  int fd = fileno(f);
  dup2(fd,1);
  args[index-2]=NULL;
  exe(args);
}

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
      
      int fd;
      int i=0;
      int flag=0;
      int flechita=0;
      char* args[255];
      
      FILE* file_temp= fopen("TEMP.txt","w+");
      int temp = fileno(file_temp); 
      token=strtok(buffer," ");
      
      char* buff= calloc(sizeof(char),255);
      while(token!=NULL){
        args[i]=token;
        token=strtok(NULL," ");
        i++;
        
        if(!strcmp(args[i-1],"|") || token == NULL){
          int pipeline[2];
          pipe(pipeline);
          pid_t nieto = fork();
          if(nieto == 0){
            close(pipeline[0]);
            if(flag == 0){
              if(token==NULL){
                args[i]=NULL;
                if(i>=2 && !strcmp(args[i-2],">")){redir(args,i);}
              }else{
                args[i-1]=NULL;
                if(i>=3 && !strcmp(args[i-3],">")){flechita=1;}
              }
            }else{
              if(token==NULL){
                lseek(temp,0,SEEK_SET);
                write(temp,buff,strlen(buff));

                args[i]="TEMP.txt";
                if(i>=2 && !strcmp(args[i-2],">")){redir(args,i);}
                args[++i] = NULL;
              }else{
                lseek(temp,0,SEEK_SET);
                write(temp,buff,strlen(buff));
                args[i-1]="TEMP.txt";
                args[i]=NULL;
                for (int j=0;j<i;j++){printf("i=%d ARG:%s\t",j,args[j]);}
                printf("\n");
                if(i>=2 && !strcmp(args[i-2],">")){flechita=1;}
              } 
            }
            if(token != NULL) dup2(pipeline[1],1);
            if(flechita){flechita=0;redir(args,i);}
            exe(args);
            
          }else{
            wait(NULL);
            close(pipeline[1]);
            lseek(pipeline[0],0,SEEK_SET);
            read(pipeline[0],buff,255);
          }
          close(pipeline[0]);
          close(pipeline[1]);
          
          flag++;
          i=0;
        }
      }
      
      free(buff); 

    }else {
    wait(&a);
    }
    free(buffer);
    remove("TEMP.txt");
  }
  
  
  
  return 0;
}