#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
  pid_t pid;
  for(int i=0;i<3;i++){
    pid=fork();
    switch(pid){
      case -1:
        perror("ERROR");
        exit(1);
      case 0:
        printf("Hijo de generacion %d creado con id: %d, id padre: %d \n",i+1, getpid(),getppid());
        break;
      default:
        printf("Padre 1 creado con id: %d",getpid());
        exit(0);

      
    }
  }

  
  exit(0);
}
