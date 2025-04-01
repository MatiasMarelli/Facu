#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>



void exe(char** args){
  if(execvp(args[0],args) == -1){printf("ERROR, arg:%s",args[0]);kill(getpid(),SIGKILL);}
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
    
    pid_t pid=fork();
    
    if(pid==0){
      
      int fd;
      int i=0;
      int flag=0;
      char* args[255];
      
      FILE* file_temp= fopen("TEMP.txt","w+");
      int temp = fileno(file_temp); 
      token=strtok(buffer," ");
      
      char* buff= calloc(sizeof(char),255);
      while(token!=NULL){
        //printf("i: %d, TOKEN %s\n",i,token);
        args[i]=token;
        token=strtok(NULL," ");
        i++;
        //if(!strcmp(">",token)) flag=0;
        if(!strcmp(args[i-1],"|") || token == NULL){
          
          //printf("\n\t | \n");
          int pipeline[2];
          pipe(pipeline);
          pid_t nieto = fork();
          if(nieto == 0){
            close(pipeline[0]);
            if(flag == 0){
              if(token==NULL){
                //printf("CASO 0\n");
                args[i]=NULL;
              }else{
                //printf("CASO 0\n");
                args[i-1]=NULL;
              }
             /*  for(int j=0;j<i;j++){
                printf("j:%d, argv:%s\n",j,args[j]);
              } */
            }else{
              if(token==NULL){

                
                //printf("CASO 1\n");
                
                lseek(temp,0,SEEK_SET);
                write(temp,buff,strlen(buff));
                //printf("write: %d\n",);
                args[i]="TEMP.txt";
                //char msg[256] = "TEMP.txt";
                //strcpy(args[i],msg);//fdopen(fd,"r");
                //args[i]=msg;
                // i++;
                
                args[++i] = NULL;
              }else{
                lseek(temp,0,SEEK_SET);
                write(temp,buff,strlen(buff));
                args[i-1]="TEMP.txt";
                args[i]=NULL;
              }

               for(int j=0;j<=i;j++){
                printf("j:%d, argv:%s\n",j,args[j]);
                //args[j]=NULL;
              } 
            }

            if(token != NULL) dup2(pipeline[1],1);
            //dup2(pipeline[1],1);

            exe(args);
            
          }else{
            wait(NULL);
            close(pipeline[1]);
            //fd=dup(pipeline[0]);
            //printf("PIPE: %d\n",fcntl(pipeline[0], F_GETFD));
            lseek(pipeline[0],0,SEEK_SET);
            int nbytes=read(pipeline[0],buff,255);
            //printf("BUFFER (Pre read):%s\n",buff);
            //printf("NBYTES = %d\n",nbytes);
            //printf("TEMP: %d\n",fcntl(temp, F_GETFD));
            //printf("write: %d\n",write(temp,buff,nbytes));

            //printf("BUFFER:%s\n",buff);
            
            //close(pipeline[0]);
            if(token==NULL){
              lseek(temp,0,SEEK_SET);
              write(temp,buff,strlen(buff));
              //printf("write: %d\n",);
            }
          }
          close(pipeline[0]);
          close(pipeline[1]);
          
          flag++;
          //printf("TOKEN DESPUES DE SALIR DE ÑIETO:%s",token);
          i=0;
          //token=strtok(NULL," ");
        }
      }
      free(buff);
     // printf("END WHILE\n");
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
      
      //remove("TEMP.txt");
    }else {
      wait(&a);
    }
    free(buffer);
    //remove("TEMP.txt");
  }


  return 0;
}