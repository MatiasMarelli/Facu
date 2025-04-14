
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


int denom = 0;
void handler(int signum){printf("ouch!\n");denom=1;}


int main() {
  
  int r;
  signal(SIGFPE,handler);
  r=1/denom;
  printf("r=%d\n",r);
  
  return 0;
}
