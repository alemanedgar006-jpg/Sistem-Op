#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

int alarma=0;
int pstree=0;
int destruir=0;

void fin_alarma(int sig){
    alarma=1;
}

void ejec_pstre(int sig){
    pstree=1;
}

void ejec_destruir(int sig){
    destruir=1;
}

void comprobar_args(int argc, char *argv[],int *segundos){
    if(argc!=2){
    perror("ERROR DE ARGUMENTOS");
    exit(1);
  }
  *segundos=atoi(argv[1]);
  if(*segundos<=0){
    perror("ERROR DE SEGUNDOS");
    exit(1);
  }
}



void gestor(pid_t root){
  pid_t pid;
  char cadena[20];
  sprintf(cadena,"%d",root);
  pid=fork();
  if(pid<0){
      perror("ERROR AL EJECUTAR PSTREE");
      exit(1);
  }
  if(pid==0){
    execlp("pstree","pstree","-c",cadena,NULL);
    perror("execlp");
    exit(1);
  }
  wait(NULL);
}

int main(int argc, char *argv[]){
  pid_t impEjec,impA;
  pid_t A,B,X,Y,Z;
  int segundos;

  comprobar_args(argc,argv,&segundos);

  impEjec=getpid();
  printf("Soy el proceso ejec mi pid es: %d \n",impEjec);
  A=fork();
  if(A<0){
      printf("ERROR al crear A");
      exit(1);
  }
  if(A==0){
      impA=getpid();
      printf("Soy el proceso A mi pid es: %d, mi padre es %d \n",impA,impEjec);
      signal(SIGUSR1,ejec_pstree);
      signal(SIGUSR2,ejec_destruir);
      B=fork();
        if(B<0){
          perror("ERROR AL crear B");
          exit(1);
        }
        if(B==0){
          printf("Soy el proceso B mi pid es: %d, mi padre es %d, mi abuelo es %d \n",getpid(),impA,impEjec);
          signal(SIGUSR2,ejec_destruir);
          X=fork();
          if(X<0){
            perror("ERROR AL CREAR X");
            exit(1);
          }
          if(X==0){
            signal(SIGUSR2,ejec_destruir);
            printf("Soy el proceso X mi pid es: %d, mi padres es %d, mi abuelo es %d, mi bisabuelo es %d \n",getpid(),getppid(),impA,impEjec);
            while(destruir==0){}
            printf("Soy X (%d) y muero \n",getpid());
            exit(0);
          }
          Y=fork();
          if(Y<0){
            perror("ERROR AL CREAR Y");
            kill(X,SIGUSR2);
            wait(NULL);
            exit(1);
          }
          if(Y==0){
            signal(SIGUSR2,ejec_destruir);
            printf("Soy el proceso Y mi pid es: %d, mi padres es %d, mi abuelo es %d, mi bisabuelo es %d \n",getpid(),getppid(),impA,impEjec);
            while(destruir==0){}
            printf("Soy Y (%d) y muero \n",getpid());
            exit(0);
          }
          Z=fork();
          if(Z<0){
            
            perror("ERROR AL CREAR Z");
            kill(X,SIGUSR2);
            kill(Y,SIGUSR2);
            wait(NULL);
            wait(NULL);
            exit(1);
          }
          if(Z==0){
            signal(SIGALRM,fin_alarma);
            signal(SIGUSR2,ejec_destruir);
            printf("Soy el proceso Z mi pid es: %d, mi padres es %d, mi abuelo es %d, mi bisabuelo es %d \n",getpid(),getppid(),impA,impEjec);
            alarm(segundos);
            while(alarma==0){}
            kill(impA,SIGUSR1);
            while(destruir==0){}
            printf("Soy Z (%d) y muero \n",getpid());
            exit(0);
          }
          while(destruir==0){}
          kill(Z,SIGUSR2);
          wait(NULL);
          kill(Y,SIGUSR2);
          wait(NULL);
          kill(X,SIGUSR2);
          wait(NULL);
            
          printf("Soy el proceso B (%d) y muero \n",getpid());      
          exit(0);
          }
      while(ejec_pstree==0){}
      gestor(impEjec);
      kill(B,SIGUSR2);
          wait(NULL);
          printf("Soy el proceso A (%d) y muero \n",impA);  
          exit(0);
  }
wait(NULL);
printf("Soy el proceso ejec (%d) y muero \n",impEjec);
  exit(0);
}
