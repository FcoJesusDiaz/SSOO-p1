#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include "definitions.h"

pid_t gp_pa = 0, gp_pb = 0, gp_pc = 0;
FILE *gp_log;

int execute_child(char *name);
void interrupt_handler(int);

int main(int argc, char **argv){
    pid_t wpid;

    if((gp_log = fopen(LOG_FILENAME, "w")) == NULL){
        fprintf(stderr, "Error al abrir el archivo log.txt\n");
        exit(EXIT_FAILURE);
    }    

    if(signal(SIGINT, interrupt_handler) == SIG_ERR){
        fprintf(stderr, "No se ha podido crear el manejador de señal\n");
    }

    gp_pa = execute_child("pa");
    wait(NULL);
    fprintf(gp_log, "Creación de directorios finalizada\n");

    gp_pb = execute_child("pb");
    gp_pc = execute_child("pc");

    while((wpid = wait(NULL)) > 0){
        if(wpid == gp_pb)  fprintf(gp_log, "Creación de modelos de examen finalizada\n");
        else fprintf(gp_log, "Creación de archivos para alcanzar la nota de corte finalizada\n");
    }
    
    fprintf(gp_log, "FIN DEL PROGRAMA\n");

    fclose(gp_log);

    return EXIT_SUCCESS;
}
 
int execute_child(char *name){
    int program_name_length = strlen(EXEC_DIR) + strlen(name) + 1;
    char *program_name = malloc(program_name_length);
    snprintf(program_name, program_name_length, "%s%s", EXEC_DIR, name);

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

    if(gp_pa) kill(gp_pa, SIGKILL);
    if(gp_pb) kill(gp_pb, SIGKILL);
    if(gp_pc) kill(gp_pc, SIGKILL);

    execute_child("pd");

    fclose(gp_log);
    exit(EXIT_SUCCESS);
}