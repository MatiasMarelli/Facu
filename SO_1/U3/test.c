#include <string.h>
#include <stdio.h>

int main(){
  char test[]="HOLA COM O ANDAS";
  /* for(int i=0;i<strlen(test);){

  } */
  char* parse=strchr(test,' ');
  printf("%s\n",parse);
  printf("%d\n",parse-test);

  return 0;
}