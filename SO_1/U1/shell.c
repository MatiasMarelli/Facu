#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int main(){
    int a;
    //pid_t pid=fork();
    const char s[2]="\n";
    char* token;
    while(1){
        
        pid_t pid=fork();
        
        if(pid==0){
            char* buffer = malloc(sizeof(char)*30);
            fgets(buffer,30,stdin);
            token=strtok(buffer,s);
            printf("Buffer:%s\n Token:%s\n",buffer,token);
            if(!strcmp(buffer,"exit")){exit(0);}



            //scanf("%[^\n]",buffer);
            //fd=open("shell.txt",'w');
            //write(fd,buffer,100);
            
            
            //bailalo ROCKY : )
            execl(token,token,NULL);
            free(buffer);
        }
        else{
            wait(&a);
        //    printf("\nclear buffer\n");
        }
    
    }
    return 0;
}