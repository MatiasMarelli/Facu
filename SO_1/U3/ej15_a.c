#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/un.h>
#include <arpa/inet.h>


#define PORT 3942

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


void handle_conn(void* sock){
  char buff[200];
  int csock = *(int*)sock;
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
  pthread_t hilo;
  csock = accept(sock,NULL,NULL);
  if(csock < 0) quit("Accept");
  
  pthread_create(&hilo,NULL,handle_conn,&csock);
  
  wait_for_client(sock);
  
  pthread_join(hilo,NULL);
  close(csock);
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