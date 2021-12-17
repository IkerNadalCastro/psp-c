#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

void manejador_senyal(int codigo_senyal);

int main() {

    signal(SIGUSR1, manejador_senyal);
    int fd[2];
    int tuberia = pipe(fd); 

    if (tuberia == -1) {

        printf("Ha ocurrido un error al crear la tubería\n"); 
        return -1; 

    }

    int id = fork(); 

    if (id == -1) {

        printf("Ha ocurrido un error al crear el fork\n"); 
        return -2; 

    } else if (id == 0) {

        close(fd[1]);
        //estamos en el hijo
        sleep(1); 
        printf("[Hijo]: He sido creado\n");
        pid_t pid;

        kill(getppid(), SIGUSR1);
        
        pause();
        
        read(fd[0], &pid, sizeof(pid_t));

        sleep(1);
        printf("[Hijo]: Acabo de leer el pid de mi abuelo\n");
        printf("[Hijo]: Soy el hijo y mi pid es %d, el de mi padre es %d, y el de mi abuelo es %d\n", getpid(), getppid(), pid);

    } else {

        int x; 
        pause();
        //estamos en el padre
        close(fd[0]); 
        pid_t pid_abuelo = getppid(); 
        write(fd[1], &pid_abuelo, sizeof(pid_t));

        printf("[Padre]: Ya he escrito el pid del abuelo en la tubería \n");

        kill(id, SIGUSR1);

        int status_hijo = wait(&x); 

        printf("[Padre]: Mi hijo ya ha terminado y este es su status de salida: %d\n", status_hijo); 

    }

}

void manejador_senyal(int codigo_senyal) {

    printf("[Hijo]: He enviado una señal a mi padre para que empiece a hacer cosas \n\n"); 

}