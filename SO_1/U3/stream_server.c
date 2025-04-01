#include <sys/socket.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAXMSG 1024
#define SRV_NAME "./SRV_NAME"

int main(){
  struct sockaddr_un srv_name,cli_name;
  socklen_t srv_size,cli_size;
  ssize_t nbytes;
  cli_size=sizeof(srv_size);
  
  char buff[MAXMSG + 1];

  int socket_srv = socket(AF_UNIX, SOCK_STREAM, 0);
  /* if(socket_srv<0){
    perror("Falló la creación del socket");
    exit(EXIT_FAILURE);
  } */

  srv_name.sun_family = AF_UNIX;
  strcpy(srv_name.sun_path,SRV_NAME);

  unlink(cli_name.sun_path);

  if (bind(socket_srv,(struct sockaddr *) &srv_name, sizeof(srv_name))){
    perror("ERROR");
    exit(EXIT_FAILURE);
  }
  listen(socket_srv, 1);

  int socket_client = accept(socket_srv, (struct sockaddr *) &cli_name,&cli_size);  

  printf("ACEPTADO\n");
  
  nbytes = recv(socket_client, buff, MAXMSG, 0);

  buff[nbytes]='\0';

  printf("[SRV]:%s\n",buff);
  

  send(socket_client, buff, nbytes, 0);

  close(socket_client);

  //close(socket_srv);

  remove(SRV_NAME);

  return 0;
}