#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void comprobar_argumentos(int argc, char *argv[],int *x, int *y){
  if(argc!=3){
    printf("ERROR DE ARGUMENTOS");
    exit(1);
  }
  *x=atoi(argv[1]);
  *y=atoi(argv[2]);
  if(x<=0||y<=0){
    printf("ARGUMENTOS INVALIDOS");
    exit(1);
  } 
}

void gestor(pid_t root){
  pid_t pid;
  char cadena[20];
  sprintf(cadena,"%d",root);
  pid=fork();
  if(pid==0){
    execlp("pstree","pstree","-c",cadena,NULL);
    perror("execlp");
    exit(1);
  }
  wait(NULL);
}

 void crear_procesos(int x, int y, pid_t root){
   
  pid_t pid;
for(int j=0;j<y;j++){
    pid=fork();
    if(pid<0){
      perror("ERROR DE FORK");
      exit(1);
    }
    if(pid==0){
      printf("Proceso creado horizontal PID=%d Padre=%d\n",getpid(), getppid());
      for(int i=2;i<=x;i++){
        pid=fork();
        if(pid<0){
          perror("ERROR DE FORK");
          exit(1);
        }
        if(pid==0){
          printf("Proceso creado vertical PID=%d Padre=%d\n",getpid(),getppid());
        }
        if(pid>0){
          wait(NULL);
          exit(0);
        }
      }
      exit(0);
    }
  }
  gestor(root);
  for(int j=0;j<y;j++){
    wait(NULL);
  }  
}

int main(int argc, char *argv[]){
  pid_t root;

  root=getpid();
  int x=0;
  int y=0;
  comprobar_argumentos(argc,argv,&x,&y);
  crear_procesos(x,y,root);

    exit(0);
}
