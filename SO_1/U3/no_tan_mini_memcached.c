#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/un.h>
#include "hash/mihash.h"
#include <signal.h>

#define PORT 4040

#define MAX_EVENTS 4

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


struct epoll_event ev, events[MAX_EVENTS];

int epoll_fd, nfds, listen_sock,csock;

HashTable table;

void* copy (void* data){
  char str[256];
  strcpy(str,(char*)data);
  return str;
}

int comp(void* data1, void* data2){
  return (strcmp((char*) data1,(char*) data2));
}

void destr(void* data){
  free(data);
}

unsigned hashi(void* data){
  return *(int*)data;
}

HashTable hashtable_create_(unsigned capacity, CopyFunction copy, CompFunction comp, DestroyFunction destr,HashFunction hash){
  HashTable table =malloc(sizeof(struct _HashTable));
  
  table->elems=malloc(sizeof(HashBox)*capacity);
  
  for(int i=0;i<capacity;i++){

    table->elems[i].data=malloc(20 * sizeof(char));
    table->elems[i].state=FREE;
  }
  
  table->numElems=0;
  table->comp=comp;table->copy=copy;table->destr=destr;table->hash=hash;table->capacity=capacity;
  return table;
}
int hashtable_nelemsm_(HashTable table){
  return table->numElems;
}
int hashtable_capacity_(HashTable table){
  return table->capacity;
}
void hashtable_destroy_(HashTable table){
  
  for(unsigned int idx=0;idx<table->capacity;idx++){
    if(table->elems[idx].state == OCCUPIED)
      (table->destr(table->elems[idx].data));
  }

  free(table->elems);
  free(table);
}
void hashtable_insert_(HashTable table,int key, void* data){
  unsigned idx=key% table->capacity;
  
  
  table->numElems++;
  table->elems[idx].data=table->copy(data);
  table->elems[idx].state=OCCUPIED;
  return;
  
}
void* hashtable_search_(HashTable table,int key){
  //unsigned idx= table->hash(data) % table->capacity;
  unsigned idx = key % table->capacity;
  void* search= NULL;
  if(table->elems[idx].state==FREE){
    search = NULL;
  }else{
    search= table->elems[idx].data;
  }
  return search;
}
void hashtable_eliminate_(HashTable table,int key){
  unsigned idx= key % table->capacity;
  
  if(table->elems[idx].state==FREE){return;}else{
    table->elems[idx].state=FREE;
    int len=strlen(table->elems[idx].data);
    for(int i = 0; i < len; i++){
      table->elems[idx].data[i] = ' ';
    }
  }
}
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

void dictionary(char** args,int csock){
  
  int key = atoi(args[1]);
  
  char* msg=malloc(sizeof(char)*100);
  if(strcmp(args[0],"PUT") == 0){
    pthread_mutex_lock(&mutex);
    hashtable_insert_(table,key,args[2]);
    pthread_mutex_unlock(&mutex);
    sprintf(msg,"INSERTANDO :%s\n",(char*)hashtable_search_(table,key));
    send(csock,msg,strlen(msg)+1,0);

  }else if(strcmp(args[0],"GET") == 0){
    pthread_mutex_lock(&mutex);
    sprintf(msg,"%s\n",(char*)hashtable_search_(table,key));
    pthre ad_mutex_unlock(&mutex);
    
    send(csock,msg,strlen(msg)+1,0);

  }else if(strcmp(args[0],"DEL") == 0){
    pthread_mutex_lock(&mutex);
    sprintf(msg,"ELIMINANDO: %s\n",(char*)hashtable_search_(table,key));
    hashtable_eliminate_(table,key);
    pthread_mutex_unlock(&mutex);
    send(csock,msg,strlen(msg)+1,0);

  }
  free(msg);
  return;
}

char* sub_str(char* s, int n){
  char* buff=malloc(sizeof(char)*50);
  int i=0;
  for(;i<n;i++){
    buff[i]=s[i];
  }
  buff[n]='\0';
  return buff;
}

char** parse(char* buffer){
  char ejemplo[50];
  char** args=calloc(sizeof(ejemplo),3);
  int j=0;
  char* parse=malloc(sizeof(char)*50);
  
  while(j<2){
    
    parse = strchr(buffer,' ');
    if(parse != NULL){

      int len= parse-buffer;
      args[j]=sub_str(buffer,len);
    }else{
      args[j]=buffer;
    }
    
    //parse=parse+1;
    
    if(j==1){
      if(strcmp(args[0],"GET")==0 || strcmp(args[0],"DEL")==0){
      }else{
        strcpy(buffer,parse+1);
      }
    }else{
      strcpy(buffer,parse+1);
    }
    
    //buffer=buffer+len;
    j++;
  }
  
  args[2]=buffer;
  
  
  return args;
}

void handle_conn(int csock){
  int rc;
  
  while(1){
    
    char* buff=malloc(sizeof(char)*200);
    rc=fd_readline(csock,buff);
    if(rc < 0) quit("Read . .. . raro");
    
    if(rc == 0){
      close(csock);
      return;
    }
    
    
    
    char** args=parse(buff);
    dictionary(args,csock);

    for(int i=0;i<3;i++){
      free(args[i]);
    }
    free(args);
  } 
}


void* wait_for_client(void* sock){
  int csock;

  csock = accept(*(int*)sock,NULL,NULL);

  if(csock < 0) quit("Accept");
  
  handle_conn(csock); 
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

void kill_server(int signum){
  hashtable_destroy_(table);
  printf("HASH CLEARED\n");
  pthread_mutex_destroy(&mutex);
  close(listen_sock);
  kill(getpid(),SIGKILL);
}

void suscripcion(){
  struct sigaction sa;
  sa.sa_handler=kill_server;
  sa.sa_flags=SA_RESTART | SA_NODEFER;
  sigaction(SIGINT,&sa,NULL);
  sigaction(SIGSEGV,&sa,NULL);
}

int main(){
  table=hashtable_create_(20,copy,comp,destr,hashi);
  suscripcion();
  listen_sock=mk_lsock();

  pthread_t t1,t2,t3,t4;
  pthread_t hilos[] = {t1,t2,t3,t4};
  epoll_fd=epoll_create1(0);
  if(epoll_fd == -1){quit("Epoll_create");}

  ev.events= EPOLLIN;
  ev.data.fd =listen_sock;

  if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,listen_sock,&ev) == -1){quit("epoll_ctl : listen_sock");}
  while(1){
    nfds=epoll_wait(epoll_fd,events,MAX_EVENTS,-1);
    if(nfds==-1){quit("epoll_wait");}
    for(int n=0;n<nfds;++n){
      if(events[n].data.fd == listen_sock){
        pthread_create(&(hilos[n]),NULL,wait_for_client, &listen_sock);
        
      }
    }
  }

  return 0;
}