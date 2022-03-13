/*Code written by Francisco Jesús Díaz Pellejero
  Email: FcoJesus.Diaz@alu.uclm.es
  Github repository source code: https://github.com/FcoJesusDiaz/SSOO-p1.git
  ---------------------------------------------------------------------------------
  Code related to the manager process. It will create processes pa, pb and pc and
  write the flow of execution in a log. In case of a Ctrl+C interruption it will
  end all of the processes and a process pd will remove the student directory*/


/*HEADERS*/
#define _POSIX_C_SOURCE 200809L /*Needed for the snprintf function*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

#include "definitions.h"

/*GLOBAL VARIABLES.*/
/*I used global variables because all of these are needed for the signal handler and you
cannot pass parameters to it.*/
pid_t g_pa = 0, g_pb = 0, g_pc = 0;
FILE *gp_log;

/*DECLARATION OF FUNCTIONS*/
int execute_child(char *name);
void interrupt_handler(int);

/*MAIN*/
int main(int argc, char **argv){
    pid_t wpid;
    int fd[2];
    float average_score;

    if((gp_log = fopen(LOG_FILENAME, "w")) == NULL){
        fprintf(stderr, "Error al abrir el archivo log.txt\n");
        exit(EXIT_FAILURE);
    }    

    if(signal(SIGINT, interrupt_handler) == SIG_ERR){
        fprintf(stderr, "No se ha podido crear el manejador de señal\n");
        exit(EXIT_FAILURE);
    }

    g_pa = execute_child("pa");
    wait(NULL);
    fprintf(gp_log, "Creación de directorios finalizada\n");
    
    /*Redirection of the write end of pipe to stdout file descriptor*/
    if(pipe(fd) == -1 || dup2(fd[PIPE_WRITE], STDOUT_FILENO) == -1){
        fprintf(stderr, "Error en el manejo de la tubería: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    g_pb = execute_child("pb");
    g_pc = execute_child("pc");

    while((wpid = wait(NULL)) > 0){
        if(wpid == g_pb)  fprintf(gp_log, "Creación de modelos de examen finalizada\n");
        else fprintf(gp_log, "Creación de archivos para alcanzar la nota de corte finalizada\n");
    }

    close(fd[PIPE_WRITE]);
    read(fd[PIPE_READ], &average_score, sizeof(average_score));
    
    fprintf(gp_log, "La nota media del primer examen es: %.2f\n", average_score);
    fprintf(gp_log, "FIN DEL PROGRAMA\n");

    fclose(gp_log);
    close(fd[PIPE_READ]);

    return EXIT_SUCCESS;
}
 
/*DEFINITION OF FUNCTIONS*/
int execute_child(char *name){
    int program_name_length = strlen(EXEC_DIR) + strlen(name) + 2;
    char *program_name = malloc(program_name_length);
    snprintf(program_name, program_name_length, "%s/%s", EXEC_DIR, name);

    int child_pid = fork();

    if(child_pid < 0){
        fprintf(gp_log, "Ha fallado la creación del proceso %s\n", name);
        exit(EXIT_FAILURE);
    }
    else if (child_pid == 0){
        execlp(program_name, name, NULL);
        fprintf(gp_log, "Error en la llamada a %s\n", name);
        exit(EXIT_FAILURE);
    }
    return child_pid;
}

void interrupt_handler(int signum){
    
    fprintf(gp_log, "Interrupción detectada, eliminado directorio estudiantes y terminando procesos...\n");

    if(g_pa) kill(g_pa, SIGKILL);
    if(g_pb) kill(g_pb, SIGKILL);
    if(g_pc) kill(g_pc, SIGKILL);

    execute_child("pd");

    fclose(gp_log);
    exit(EXIT_SUCCESS);
}