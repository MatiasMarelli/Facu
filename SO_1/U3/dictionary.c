#include "hash/mihash.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <sys/wait.h>


void* copy (void* data){
  return data;
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

HashTable table;

int* loop;
int* sig;



HashTable hashtable_create_(unsigned capacity, CopyFunction copy, CompFunction comp, DestroyFunction destr,HashFunction hash){
  HashTable table =malloc(sizeof(struct _HashTable));
  assert(table!=NULL);
  table->elems=malloc(sizeof(HashBox)*capacity);
  assert(table->elems!=NULL);
  for(int i=0;i<capacity;i++){
    //table->elems[i].data=NULL;

    table->elems[i].data=malloc(20*sizeof(char));
    table->elems[i].state=FREE;
  }
  /* for(int i=0;i<capacity;i++){
    table->elems[i].data=glist_crear();
  } 
  */
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
  //unsigned idx= table->hash(data)% table->capacity;
  unsigned idx=key% table->capacity;
  //insertar dato si esta libre la casilla

  
  //if((table->numElems/table->capacity)*1.0>0.7){table=hashtable_redim(table);}
  table->numElems++;
  table->elems[idx].data=table->copy(data);
  table->elems[idx].state=OCCUPIED;
  /* glist_agregar_final(table->elems[idx].data,table->copy(data)); */
  return;
  

  //sobreescribir el dato si el mismo ya se encontraba en la tabla
  

  /* if(table->comp(table->elems[idx].data,data)==0){
    if((table->numElems/table->capacity)*1.0>0.7){table=hashtable_redim(table);}
    table->destr(table->elems[idx].data);
    table->elems[idx].data=table->copy(data);
    return;
  } */
  /* if(table->comp(table->elems[idx].data->data, data)==0){
    table->destr(table->elems[idx].data->data);
    glist_agregar_final(table->elems[idx].data,table->copy(data)); 
    return;
  }
    */
  //en caso de colision no asemo na :()
  /*
  else{
    if((table->numElems/table->capacity)*1.0>0.7){table=hashtable_redim(table);}
    
    /* glist_agregar_final(table->elems[idx].data,data); 
    for(int a=0;a!=table->capacity;idx=(idx+1)%table->capacity){
      if(table->elems[idx].data==NULL){
        table->numElems++;
        table->elems[idx].data=table->copy(data);
        
      }
      a++;
    }
    return;
  } 
  */ 
  
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
  /* if(table->elems[idx].data==NULL){
    
  }else{

    /* if(glist_buscar(table->elems[idx].data,data,table->comp)==0){return data;} 
    int a=0;
    for(;a!=table->capacity;idx=(idx+1)%table->capacity){
      if(table->elems[idx].state==OCCUPIED){
        if(table->comp(table->elems[idx].data,data)==0){
          a=table->capacity-1;
          search = table->elems[idx].data;
        }
      }
      a++;
    }
  } 
  */
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


  
  /* 
  int a=0;
  for(;a!=table->capacity;idx=(idx+1)%table->capacity){
    if(table->elems[idx].state==OCCUPIED){

      if(table->comp(table->elems[idx].data,data)==0){
        a=table->capacity;
        table->elems[idx].state=FREE;
        table->destr(table->elems[idx].data);
      }
    }
    a++;
  } */
  /* if(glist_buscar(table->elems[idx].data,data,table->comp)==0){
    table->destr(table->elems[idx].data);
    table->numElems--;
    table->elems[idx].data=NULL;
  } 
  return;
  */
}

#define PORT 4040

int sock;

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

char** parse(int* i,char* buff,char** args){
  *i =0;
  buff=strtok(buff," ");
  while(buff!=NULL){
    args[*i]=buff;
    buff=strtok(NULL, " "); 
    (*i)++;
  }


  return args;
}

void dictionary(HashTable table, char** args,int csock){
  int key = atoi(args[1]);
  char msg[100];
  if(strcmp(args[0],"PUT") == 0){
    hashtable_insert_(table,key,args[2]);
    sprintf(msg,"INSERTANDO :%s\n",(char*)hashtable_search_(table,key));
    send(csock,msg,strlen(msg)+1,0);
  }else if(strcmp(args[0],"GET") == 0){
    sprintf(msg,"%s\n",(char*)hashtable_search_(table,key));
    send(csock,msg,strlen(msg)+1,0);
  }else if(strcmp(args[0],"DEL") == 0){
    sprintf(msg,"ELIMINANDO: %s\n",(char*)hashtable_search_(table,key));
    send(csock,msg,strlen(msg)+1,0);
    hashtable_eliminate_(table,key);
  }else{
    raise(SIGINT);
  }
  return;

}
void client_request(int* pipe,HashTable table,int csock){
  char* buff = malloc(sizeof(char)*255);
  int* i = malloc(sizeof(int));
  char example[20];
  read(pipe[0],buff,255);
  char** args=malloc(sizeof(example)*10);
  printf("CLIENT REQUEST\n");
  args=parse(i,buff,args);
  dictionary(table,args,csock);
  free(args);
}


void handle_conn(int csock,int* pipe){
  char buff[200];
  int rc;
  
  
  
  while(1){
    rc=fd_readline(csock,buff);
    if(rc < 0) quit("Read . .. . raro");
    
    if(rc == 0){
      kill(getppid(),SIGUSR2);
      close(csock);
      return;
    }
    write(pipe[1],buff,strlen(buff)+1);
    kill(getppid(),SIGUSR1);
  }
}


void wait_for_client(int sock,HashTable table){
  int csock;

  csock = accept(sock,NULL,NULL);
  if(csock < 0) quit("Accept");

  int pipeline[2];
  pipe(pipeline);
  printf("Pre FORK\n");
  pid_t pid = fork();

  if(!pid){
  close(pipeline[0]);
  handle_conn(csock,pipeline);
  exit(0);
    
  }else{
    close(pipeline[1]);
  
    while(loop){
      pause();
      if(*sig){
        client_request(pipeline,table,csock);
        *sig=0;
      }
    } 
    *loop=1;
    close(csock);
    wait(NULL);
    wait_for_client(sock,table);

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

void child_call(int signum){
  *sig=1;
  return;
}
void end_loop(int signum){*loop=0;return;}

void kill_server(int signum){
  hashtable_destroy_(table);
  close(sock);
  kill(getpid(),SIGKILL);
}

int main(){
  printf("HOLA\n");
  
  table = hashtable_create_(20,copy,comp,destr,hashi);
  struct sigaction sa1, sa2, sa3;
  
  
  sa3.sa_flags=SA_RESTART | SA_NODEFER;
  sa2.sa_flags=SA_RESTART | SA_NODEFER;
  sa1.sa_flags=SA_RESTART | SA_NODEFER;
  
  
  sa1.sa_handler=child_call;
  sa2.sa_handler=end_loop;
  sa3.sa_handler=kill_server;
  
  
  sigaction(SIGUSR1,&sa1,NULL);
  sigaction(SIGUSR2,&sa2,NULL);
  sigaction(SIGINT,&sa3,NULL);
  sigaction(SIGSEGV,&sa3,NULL);

  loop = malloc(sizeof(int));
  sig = malloc(sizeof(int));
  *loop=1;
  *sig=0;
  
  sock=mk_lsock();
  
  wait_for_client(sock,table);
  

  return 0;
} 