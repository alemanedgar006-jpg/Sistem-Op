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
        exit(-1);

      
    }
  }

  
  exit(0);
}
