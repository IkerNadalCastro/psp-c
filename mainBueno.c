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

        printf("Ha ocurrido un error en la creación del pipe \n");
        return -1; 

    }

    pid_t pid = fork(); 

    if (pid == -1) {

        printf("Ha ocurrido un error en la creación del fork \n");
        return -1; 

    } else if (pid == 0) {

        //estamos en el hijo

        close(fd[1]);
        sleep(1); 

        printf("[Hijo]: Acabo de ser creado\n");

        pid_t pidAbuelo; 

        kill(getppid(), SIGUSR1); 

        pause();  

        read(fd[0], &pidAbuelo, sizeof(pid_t));
        printf("[Hijo]: Acabo de recibir el PID de mi abuelo \n"); 

        sleep(1); 

        printf("[Hijo]: Mi pid es %d, el de mi padre es %d y el de mi abuelo es %d \n", getpid(), getppid(), pidAbuelo);
        close(fd[0]); 

    } else {

        //estamos en el padre
        pid_t pid_abuelo; 
        
        pause();
        
        close(fd[0]); 
        pid_abuelo = getppid();
        write(fd[1], &pid_abuelo, sizeof(pid_t));

        printf("[Padre]: Le acabo de escribir a mi hijo el PID de mi abuelo\n");
        kill(pid, SIGUSR1); 

        int status_hijo; 
        wait(&status_hijo); 
        int statusSalida = WEXITSTATUS(status_hijo);

        printf("[Padre]: Mi hijo ya acabado y este es su status de salida: %d \n", statusSalida); 
        close(fd[1]);  

        return 0; 
        
    }

}

void manejador_senyal(int codigo_senyal) {

    printf("Se ha enviado una señal\n"); 

}
