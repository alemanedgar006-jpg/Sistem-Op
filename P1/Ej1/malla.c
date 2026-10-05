#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


void gestor(){
  pid_t pid;
  pid=fork();
  if(pid==0){
    execlp("pstree","pstree","-c",NULL);
    perror("execlp");
    exit(1);
  }
  wait(NULL);
}

int main(int argc, char *argv[]){
  pid_t root;
  pid_t pid;

  root=getpid();
  int x,y;
  if(argc!=3){
    printf("ERROR DE ARGUMENTOS");
    exit(1);
  }
  x=atoi(argv[1]);
  y=atoi(argv[2]);
  
  if(x<=0||y<=0){
    printf("ARGUMENTOS INVALIDOS");
    exit(1);
  }

  for(int j=0;j<y;j++){
    pid=fork();
    if(pid<0){
      perror("ERROR DE FORK");
      exit(1);
    }
    if(pid==0){
      printf("Proceso creado PID=%d Padre=%d\n",getpid(), getppid());
      for(int i=2;i<=x;i++){
        pid=fork();
        if(pid<0){
          perror("ERROR DE FORK");
          exit(1);
        }
        if(pid>0){
          wait(NULL);
          exit(0);
        }
      }
      exit(0);
    }
  }
  gestor();
  for(int j=0;j<y;j++){
    wait(NULL);
  }

  exit(0);
}
