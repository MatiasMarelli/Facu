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


#define PORT 4040
#define MAX_EVENTS 4

struct epoll_event ev, events[MAX_EVENTS];

int epoll_fd, nfds, listen_sock,csock;

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


void wait_for_client(void* sock){
  
  
  
  csock = accept(*(int*)sock,NULL,NULL);
  if(csock < 0) quit("Accept");
  
  //pthread_create(&hilo,NULL,handle_conn,&csock);
  handle_conn(&csock);

  close(csock);
 // wait_for_client(sock);
  
  //pthread_join(hilo,NULL);
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

/* typedef union epoll_data{
  void* ptr;
  int fd;
  uint32_t u32;
  uint64_t u64;
}epoll_data_t; 

struct epoll_event{
  uint32_t events;
  epoll_data_t data;
}; */





int main(){

  listen_sock=mk_lsock();

  pthread_t t1,t2,t3,t4;
  pthread_t hilos[] = {t1,t2,t3,t4};
  epoll_fd=epoll_create1(0);
  if(epoll_fd == -1){quit("Epoll_create");}

  ev.events= EPOLLIN;
  ev.data.fd =listen_sock;

  if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,listen_sock,&ev) == -1){quit("epoll_ctl : listen_sock");}
  while(1){
    nfds=epoll_wait(epoll_fd,events,MAX_EVENTS,20000);
    if(nfds==-1){quit("epoll_wait");}
    for(int n=0;n<nfds;++n){
      if(events[n].data.fd == listen_sock){
        pthread_create(&(hilos[n]),NULL,wait_for_client, &listen_sock);
        //pthread_join(hilos[n],NULL);
      }
    }
  }

  //wait_for_client(sock);

  return 0;
}