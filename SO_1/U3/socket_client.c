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
  int src= socket(AF_UNIX,SOCK_DGRAM,0);
  int client= socket(AF_UNIX,SOCK_DGRAM,0);
  
  struct sockaddr_un srv_name, client_name;
  srv_name.sun_family = AF_UNIX;
  strcpy (srv_name.sun_path,"SRV_NAME");

  client_name.sun_family = AF_UNIX;
  strcpy (client_name.sun_path,"SRV_NAME");

  socklen_t srv_size = sizeof(srv_name);  
  
  unlink(client_name.sun_path);

  strcpy (buffer,MSG);
  //bind(src,(struct sockaddr *) &srv_name, sizeof(srv_name));

  bind(client,(struct sockaddr *) &client_name, sizeof(client_name));
  sendto(client,buffer,strlen(buffer), 0,(struct sockaddr *) &srv_name, srv_size);

  printf("[CLI] SENDTO...\n");

  int nbytes = recvfrom(client,buffer, sizeof(char)*255 , 0 ,(struct sockaddr *) &srv_name, &srv_size);

  printf("[SRV] recv: %s\n",buffer);


  printf("[SRV] SEND OK\n");

  close(src);
  
  return 0;
}