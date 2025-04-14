#include <sys/socket.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAXMSG 1024
#define CLI_NAME "./CLIENT"
#define MSG "Colorado bobo"
#define SRV_NAME "./SRV_NAME"

int main(){
  int sock_cli; 
  struct sockaddr_un srv_name, cli_name;
  
  ssize_t nbytes;
  char buff [MAXMSG+1];
  socklen_t srv_size;

  
  
  sock_cli=socket(AF_UNIX,SOCK_STREAM,0);
  printf("SOCKETEADO\n");
  if(sock_cli<0){
    perror("FALLO\n");
    exit(EXIT_FAILURE);
  }

  cli_name.sun_family= AF_UNIX ;
  strncpy(cli_name.sun_path,CLI_NAME,sizeof(cli_name.sun_path));

  
  unlink(cli_name.sun_path);

  if( (bind(sock_cli, (struct sockaddr*) & cli_name , sizeof(cli_name))) < 0 ){
    perror("Falló la asignación de nombre");
    exit(EXIT_FAILURE);
  }

  srv_name.sun_family=AF_UNIX;
  strcpy(srv_name.sun_path,SRV_NAME);
  srv_size=sizeof(struct sockaddr_un);
  

  if(connect(sock_cli, (struct sockaddr *) &srv_name,srv_size)){
    perror("Falló el intento de conexión con el servidor");
    exit(EXIT_FAILURE);
  }
  
  
  send(sock_cli, MSG, strlen(MSG), 0);
  printf("Enviado...\n");
  
  nbytes=recv(sock_cli,buff,MAXMSG,0);
  if(nbytes<0){
    perror("Fallo al recibir mensaje de vuelta...");
    exit(EXIT_FAILURE);
  }
  buff[nbytes] = '\0';

  printf("[CLIENT] Llegó >%s<\n", buff);
  
  close(sock_cli);
  
  remove(CLI_NAME);

  return 0;

}