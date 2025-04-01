#include <sys/socket.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MSG "Colorado bobo"

int main(){

  char buffer[255];
  socklen_t client_size, srv_size;
  int srv= socket(AF_UNIX,SOCK_DGRAM,0);
  int client= socket(AF_UNIX,SOCK_DGRAM,0);
  
  struct sockaddr_un srv_name, client_name;
  srv_name.sun_family = AF_UNIX;
  strcpy (srv_name.sun_path,"SRV_NAME");

  client_name.sun_family = AF_UNIX;
  strcpy (client_name.sun_path,"SRV_NAME");

  socklen_t srv_size = sizeof(srv_name);  

  unlink(srv_name.sun_path);

  strcpy (buffer,MSG);
  bind(srv,(struct sockaddr *) &srv_name, sizeof(srv_name));

  //bind(client,(struct sockaddr *) &client_name, sizeof(client_name));
  sendto(srv,buffer,strlen(buffer), 0,(struct sockaddr *) &srv_name, srv_size);

  printf("[CLI] RCVFROM...\n");

  int nbytes = recvfrom(client,buffer, sizeof(char)*255 , 0 ,(struct sockaddr *) &srv_name, &srv_size);

  buffer[nbytes]='\0';

  printf("[SRV] recv: %s\n",buffer);


  close(srv);

  remove("./SRV_NAME");
  
  return 0;
}