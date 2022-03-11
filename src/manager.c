#include "definitions.h"

int execute_child(char *name, FILE *log_fp);
void interrupt_handler(int sig_num, FILE *log_fp, pid_t *pa, pid_t *pb, pid_t *pc);

int main(int argc, char **argv){
    pid_t *pa = NULL, *pb = NULL, *pc = NULL, wpid;
    FILE *log_fp;

    signal(SIGTERM, interrupt_handler);

    if((log_fp = fopen(LOG_FILENAME, "w")) == NULL){
        fprintf(stderr, "Error al crear el archivo log.txt\n");
        exit(EXIT_FAILURE);
    }
    *pa = execute_child("pa", log_fp);
    wait(NULL);
    fprintf(log_fp, "Creación de directorios finalizada\n");

    *pb = execute_child("pb", log_fp);
    *pc = execute_child("pc", log_fp);

    while((wpid = wait(NULL)) > 0){
        if(wpid == pb)  printf("Creación de modelos de examen finalizada\n");
        else printf("Creación de archivos para alcanzar la nota de corte finalizada\n");
    }
    
    fprintf(log_fp, "FIN DEL PROGRAMA");
    return EXIT_SUCCESS;
}

int execute_child(char *name, FILE *log_fp){
    int program_name_length = strlen(EXEC_DIR) + strlen(name) + 1;
    char *program_name = malloc(program_name_length);
    snprintf(program_name, program_name_length, "%s%s", EXEC_DIR, name);

    int child_pid = fork();

    if(child_pid < 0){
        fprintf(log_fp, "Ha fallado la creación del proceso %s", name);
        exit(EXIT_FAILURE);
    }
    else if (child_pid == 0){
        execlp(program_name, name, NULL);
        fprintf(log_fp, "Error en la llamada a %s\n", name);
        exit(EXIT_FAILURE);
    }
    return child_pid;
}

void interrupt_handler(int sig_num, FILE *log_fp, pid_t *pa, pid_t *pb, pid_t *pc){
    if(pa) kill(pa, SIGKILL);
    if(pb) kill(pb, SIGKILL);
    if(pc) kill(pc, SIGKILL);
    execute_child("pd", log_fp);
    fprintf("Interrupción detectada, eliminado directorio estudiantes y terminando procesos...");
}