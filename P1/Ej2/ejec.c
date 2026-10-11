#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

int alarma=0;
int pstree=0;
int destruir=0;
int segundos;
pid_t impA,impEjec;

void fin_alarma(int sig){
    alarma=1;
}

void ejec_pstree(int sig){
    pstree=1;
}

void ejec_destruir(int sig){
    destruir=1;
}

void proceso_A();
void proceso_B();
void proceso_X();
void proceso_Y();
void proceso_Z();
void gestor(pid_t root);

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

void crear_A(){
    pid_t A=fork();
    if(A<0){
      printf("ERROR al crear A");
      exit(1);
    }
    if(A==0){
        proceso_A();
        exit(0);
    }
}

pid_t crear_B(){
    pid_t B=fork();
    if(B<0){
        perror("ERROR al crear B");
        exit(1);
    }
    if(B==0){
        proceso_B();
        exit(0);
    }
    return B;
}

pid_t crear_X(){
    pid_t X=fork();
    if(X<0){
        perror("ERROR al crear X");
        exit(1);
    }
    if(X==0){
        proceso_X();
        exit(0);
    }
    return X;
}

pid_t crear_Y(){
pid_t Y=fork();
    if(Y<0){
        perror("ERROR al crear Y");
        exit(1);
    }
    if(Y==0){
        proceso_Y();
        exit(0);
    }
    return Y;
}

pid_t crear_Z(){
    pid_t Z=fork();
    if(Z<0){
        perror("ERROR al crear Z");
        exit(1);
    }
    if(Z==0){
        proceso_Z();
        exit(0);
    }    
    return Z;
}

void proceso_ejec(){
    impEjec;
    impEjec=getpid();
    printf("Soy el proceso ejec mi pid es: %d \n",impEjec);
    crear_A();
    wait(NULL);
    printf("Soy ejec (%d) y muero \n",impEjec);
    
}


void proceso_A(){
    pid_t B;
    impA=getpid();
    printf("Soy el proceso A mi pid es: %d, mi padre es %d \n",impA,impEjec);
    signal(SIGUSR1,ejec_pstree);
    signal(SIGUSR2,ejec_destruir);
    B=crear_B();
    while(pstree==0){}
    gestor(impEjec);
    kill(B,SIGUSR2);
    wait(NULL);
    printf("Soy A (%d) y muero \n",impA);  
    exit(0);
}

void proceso_B(){
    pid_t X,Y,Z;
    printf("Soy el proceso B mi pid es: %d, mi padre es %d, mi abuelo es %d \n",getpid(),impA,impEjec);
    signal(SIGUSR2,ejec_destruir);
    X=crear_X();
    Y=crear_Y();
    Z=crear_Z();
    kill(impA,SIGUSR1);
    while(destruir==0){}
    kill(Z,SIGUSR2);
    wait(NULL);
    kill(Y,SIGUSR2);
    wait(NULL);
    kill(X,SIGUSR2);
    wait(NULL);
            
    printf("Soy B (%d) y muero \n",getpid());      
    exit(0);
}

void proceso_X(){
    signal(SIGUSR2,ejec_destruir);
    printf("Soy el proceso X mi pid es: %d, mi padres es %d, mi abuelo es %d, mi bisabuelo es %d \n",getpid(),getppid(),impA,impEjec);
    while(destruir==0){}
    printf("Soy X (%d) y muero \n",getpid());
}

void proceso_Y(){
    signal(SIGUSR2,ejec_destruir);
    printf("Soy el proceso Y mi pid es: %d, mi padres es %d, mi abuelo es %d, mi bisabuelo es %d \n",getpid(),getppid(),impA,impEjec);
    while(destruir==0){}
    printf("Soy Y (%d) y muero \n",getpid());
}

void proceso_Z(){
    signal(SIGALRM,fin_alarma);
    signal(SIGUSR2,ejec_destruir);
    printf("Soy el proceso Z mi pid es: %d, mi padres es %d, mi abuelo es %d, mi bisabuelo es %d \n",getpid(),getppid(),impA,impEjec);
    alarm(segundos);
    while(alarma==0){}
    while(destruir==0){}
    printf("Soy Z (%d) y muero \n",getpid());
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
    comprobar_args(argc,argv,&segundos);
    proceso_ejec();
    exit(0);
}
