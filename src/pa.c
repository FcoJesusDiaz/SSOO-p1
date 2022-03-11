#include "definitions.h"

void create_directory(char *name);
int main(int argc, char **argv){
    FILE *fptr = fopen(STUDENTS_INFO, "r");
    char *dni = NULL;
    char *lineptr = malloc(MAX_LINE);
    if(!(fptr && lineptr)){
        return EXIT_FAILURE;
    }
    while(fgets(lineptr, MAX_LINE, fptr)){
        dni = strtok(lineptr, " ");
        if(dni){
            create_directory(dni);
        }
    }
    free(lineptr);
    return EXIT_SUCCESS;
}

void create_directory(char *name){
    char *directory_name = malloc(strlen(STUDENTS_DIR) + strlen(name) + 1);
    strcpy(directory_name, STUDENTS_DIR);
    strcat(directory_name, name);
    DIR* dir = opendir(name);
    if (!dir){
        mkdir(directory_name, 0700);
    }
}
