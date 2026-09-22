#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
  pid_t pid;
  for(int i=0;i<3;i++){
    pid=fork();
    switch(pid){
      case -1:
        perror("Error");
        exit(1);
      case 0:
        printf("Hijo creado, PID: %d, padre: %d\n", getpid(),getppid());
        exit(0);
      default:
        printf("Padre ha creado proceso n: %d, con PID: %d\n",i+1,pid);
        break;
    } 
  }
  exit(0);
}
