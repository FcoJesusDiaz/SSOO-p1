/*Code written by Francisco Jesús Díaz Pellejero
  Email: FcoJesus.Diaz@alu.uclm.es
  Github repository source code: https://github.com/FcoJesusDiaz/SSOO-p1.git
  ---------------------------------------------------------------------------------
  Code related to the pb process. It will create symbolic links to the exam model that 
  each student has to take.*/


/*HEADERS*/
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "definitions.h"

/*DECLARATION OF FUNCTIONS*/
void copy_model(char *dni, char *model);

/*MAIN*/
int main(int argc, char **argv){
    FILE *fptr = fopen(STUDENTS_INFO, "r");
    char *dni = NULL;
    char *model = NULL;
    char *lineptr = malloc(MAX_LINE);

    while(fgets(lineptr, MAX_LINE, fptr)){
        if((dni = strtok(lineptr, " ")) && (model = strtok(NULL, " "))){
            copy_model(dni, model);
        }
    }
    free(lineptr);
    return EXIT_SUCCESS;
}


/*DEFINITION OF FUNCTIONS*/
void copy_model(char *dni, char *model){
    int pdf_file_length = strlen("Modelo") + strlen(model) + strlen(".pdf") + 1;
    int target_name_length = strlen(EXAMS_DIR) + pdf_file_length;
    int linkpath_length = strlen(STUDENTS_DIR) + strlen(dni) + pdf_file_length + 1;

    char *target = malloc(target_name_length);
    char *linkpath = malloc(linkpath_length);

    snprintf(target, target_name_length,"%sModelo%s.pdf", EXAMS_DIR, model);
    snprintf(linkpath, linkpath_length, "%s%s/Modelo%s.pdf", STUDENTS_DIR, dni, model);
    
    symlink(target, linkpath);

    free(target);
    free(linkpath);
}
