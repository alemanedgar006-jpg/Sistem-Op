#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  pid_t impEjec,impA;
  pid_t A,B,X,Y,Z;
  int segundos;
  if(argc!=2){
    perror("ERROR DE ARGUMENTOS");
    exit(1);
  }
  segundos=atoi(argv[1]);
  if(segundos<=0){
    perror("ERROR DE SEGUNDOS");
    exit(1);
  }
  impEjec=getpid();
  printf("Soy el proceso ejec mi pid es: %d \n",impEjec);
  A=fork();
  switch(A){
    case -1:
      printf("ERROR al crear A");
      exit(1);
    case 0:
      impA=getpid();
      printf("Soy el proceso A mi pid es: %d, mi padre es %d \n",impA,impEjec);
      B=fork();
      if(B<0){
        perror("ERROR AL crear B");
        exit(1);
      }
      if(B==0){
        printf("Soy el proceso B mi pid es: %d, mi padre es %d, mi abuelo es %d \n",getpid(),impA,impEjec);
        X=fork();
        if(X<0){
          perror("ERROR AL CREAR X");
          exit(1);
        }
        else{
          printf("Soy el proceso X mi pid es: %d, mi padres es %d, mi abuelo es %d, mi bisabuelo es %d \n",getpid(),getppid(),impA,impEjec);
        }
        Y=fork();
        if(Y<0){
          perror("ERROR AL CREAR Y");
          exit(1);
        }
        else{
          printf("Soy el proceso Y mi pid es: %d, mi padres es %d, mi abuelo es %d, mi bisabuelo es %d \n",getpid(),getppid(),impA,impEjec);
        }
        Z=fork();
        if(Z<0){
          perror("ERROR AL CREAR Z");
          exit(1);
        }
        else{
          printf("Soy el proceso Z mi pid es: %d, mi padres es %d, mi abuelo es %d, mi bisabuelo es %d \n",getpid(),getppid(),impA,impEjec);
        }
        
      }
  }
  exit(0);
}
