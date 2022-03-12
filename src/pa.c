#include "definitions.h"

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>


void create_directory(char *name);

int main(int argc, char **argv){
    FILE *fptr = fopen(STUDENTS_INFO, "r");
    char *dni = NULL;
    char *lineptr = malloc(MAX_LINE);

    while(fgets(lineptr, MAX_LINE, fptr) && (dni = strtok(lineptr, " "))){    
        create_directory(dni);
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
        mkdir(directory_name, 0770);
    }
}
