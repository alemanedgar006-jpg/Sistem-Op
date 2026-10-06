#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc char *argv[]){
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

  
}
