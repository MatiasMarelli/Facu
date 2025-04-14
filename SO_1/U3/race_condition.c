#include  <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void characterAtTime(char* str){
  setbuf(stdout,NULL); //no buffering
  char* ptr;
  int c;
  for(ptr=str;(c=*ptr++)!=0;){
    putc(c,stdout);
  }

}


int main(){

  pid_t pid= fork();

  if(pid==0){
    characterAtTime("output from child y otras cosas bieeeennnn turbiass\n");
  }else{
    characterAtTime("output from parent y el divorcio se aproxima\n");
  }

  return 0;
}