/*Code written by Francisco Jesús Díaz Pellejero
  Email: FcoJesus.Diaz@alu.uclm.es
  Github repository source code: https://github.com/FcoJesusDiaz/SSOO-p1.git
  ---------------------------------------------------------------------------------
  Code related to the pa process. It will create directories for each student.*/


/*HEADERS*/
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "definitions.h"

/*DEFINITION OF FUNCTIONS*/
void create_directory(char *name);


/*MAIN*/
int main(int argc, char **argv){
    FILE *fptr = fopen(STUDENTS_INFO, "r");
    char *dni = NULL;
    char *lineptr = malloc(MAX_LINE);

    /*Creates students directory if it does not exists*/
    if(!opendir(STUDENTS_DIR)) mkdir(STUDENTS_DIR, 0770);

    while(fgets(lineptr, MAX_LINE, fptr) && (dni = strtok(lineptr, " "))){    
        create_directory(dni);
    }

    free(lineptr);
    return EXIT_SUCCESS;
}


/*DECLARATION OF FUNCTIONS*/
void create_directory(char *name){
    char *directory_name = malloc(strlen(STUDENTS_DIR) + strlen(name) + 1);
    strcpy(directory_name, STUDENTS_DIR);
    strcat(directory_name, name);
    DIR* dir = opendir(name);
    if (!dir){
       mkdir(directory_name, 0770);
    }
}
