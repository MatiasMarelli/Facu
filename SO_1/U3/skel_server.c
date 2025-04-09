
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>


#define PORT 4040

int U=0;
  
void quit(char* s){
  perror(s);
  exit(-1);
}

int fd_readline(int fd, char* buff){
  int rc;
  int i=0;

  while((rc = read(fd,buff + i, 1)) > 0){
    if(buff[i] =='\n') break;
    i++;

  }
  if(rc < 0) return rc;

  buff[i]=0;
   
  return i;
}


void handle_conn(int csock){
  char buff[200];
  int rc;
  
  while(1){
    rc=fd_readline(csock,buff);
    if(rc < 0) quit("Read . .. . raro");

    if(rc == 0){
      close(csock);
      return;
    }

    if(!strcmp(buff,"NUEVO")){
      char reply[20];
      sprintf(reply,"%d\n", U);
      U++;
      write(csock,reply,strlen(reply));

    }else if(!strcmp(buff,"CHAU")){
      close(csock);
      return;
    }
  }
}


void wait_for_client(int sock){
  int csock;

  csock = accept(sock,NULL,NULL);
  if(csock < 0) quit("Accept");
  
  pid_t pid = fork();

  if(!pid){
  handle_conn(csock);
  exit(0);
    
  }else{ 
    close(csock);
    wait(NULL);
    wait_for_client(sock);

  }
}


int mk_lsock(){
  int sock;

  sock =socket(AF_INET,SOCK_STREAM,0);
  if(sock<0) quit("Socket");

  struct sockaddr_in sa;
  sa.sin_family = AF_INET;
  sa.sin_port = htons(PORT);
  sa.sin_addr.s_addr = htonl(INADDR_ANY);

  int b =bind(sock, (struct sockaddr*)&sa, sizeof sa);

  if(b<0) quit("Bind");

  b=listen(sock,10);

  if(b<0) quit("Listen");
  
  return sock;
}

int main(){

  int sock;

  sock=mk_lsock();

  wait_for_client(sock);

  return 0;
}