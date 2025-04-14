#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

int seek_write(int fd, off_t offset, char* buffer){
  lseek(fd,offset,SEEK_SET);
  int bytes= write(fd,buffer,strlen(buffer)+1);
  return bytes;
}

int seek_read(int fd, off_t offset, char* buffer,int size){
  lseek(fd,offset,SEEK_SET);
  int bytes = read(fd,buffer,size);
  return bytes;
}


int main(){

  /* FILE* f = fopen("./test.txt","w+");
  int fd=fileno(f); */
  int fd=open("./test.txt",O_RDWR);
  char hola[]="Me pican los cocos\n";
  char hola2[20];
  int bytes= seek_write(fd,20,hola);
 // int bytes=seek_read(fd,40,hola2,20); 
  printf("\n%s\n",hola2);
  close(fd);



  return 0;
}