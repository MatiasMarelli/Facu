#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void usr_handler(int signum){
  printf("Gettid:%d\n",gettid());
  printf("Getgid:%d\n",getgid());
  printf("Pid:%d esta entrando en bucle..\n",getpid());
  while(1);
}


int main(){

  pid_t pid = fork();
  if(!pid){
    printf("HOLA\n");
    struct sigaction child;
    child.sa_flags=SA_NODEFER | SA_RESTART;
    child.sa_handler=usr_handler;
    printf("%d\n",sigaction(SIGUSR1,&child,NULL));
    printf("Handler de child configurado...\n");
    pause();
  }else{  
    sleep(2);
    printf("Parent pid:%d\n",getpid());
    /* struct sigaction parent;
    parent.sa_flags=SA_RESTART | SA_NODEFER;
    parent.sa_handler=usr_handler; */
    printf("Handler de parent configurado...\n");
    //sigaction(SIGUSR1,&parent,NULL);
    signal(SIGUSR1,usr_handler);
    kill(pid,SIGUSR1);
    sleep(5);
    printf("Signal enviada a child\n");
    //  sleep(2);
    raise(SIGUSR1);
  }

  return 0;
}