/*Code written by Francisco Jesús Díaz Pellejero
  Email: FcoJesus.Diaz@alu.uclm.es
  Github repository source code: https://github.com/FcoJesusDiaz/SSOO-p1.git
  ---------------------------------------------------------------------------------
  Code related to the pc process. It will compute the neccesary grade for each student and
  it will send the average score of the first exam through a pipe. The write end of
  the pipe is the standard output. */

/*HEADERS*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "definitions.h"

/*DECLARATION OF FUNCTIONS*/
void compute_neccesary_grade(char *dni, char *model, char *first_grade);


/*MAIN*/
int main(int argc, char **argv){
    FILE *fptr = fopen(STUDENTS_INFO, "r");
    char *dni = NULL;
    char *model = NULL;
    char *first_grade = NULL;
    char *lineptr = malloc(MAX_LINE);

    int nStudents = 0;
    float sum_of_grades = 0;
    float average = 0;

    while(fgets(lineptr, MAX_LINE, fptr)){
        if((dni = strtok(lineptr, " ")) && (model = strtok(NULL, " ")) && (first_grade = strtok(NULL, " "))){
            compute_neccesary_grade(dni, model, first_grade);
            sum_of_grades += atoi(first_grade);
            nStudents++;
        }
    }

    average = sum_of_grades/nStudents;
    write(STDOUT_FILENO, &average, sizeof(average));
    free(lineptr);
    return EXIT_SUCCESS;
}

/*DEFINITION OF FUNCTIONS*/
void compute_neccesary_grade(char *dni, char *model, char *first_grade){
    FILE *fp;
    int neccesary_grade = NUM_EXAMS * MIN_GRADE - atoi(first_grade);
    int pathname_length = strlen(STUDENTS_DIR) + strlen(dni) + strlen("/") + strlen(MIN_GRADE_FILENAME) + 1;
    char *pathname = malloc(pathname_length);

    snprintf(pathname, pathname_length, "%s%s/%s", STUDENTS_DIR, dni, MIN_GRADE_FILENAME);
    
    if((fp = fopen(pathname, "w")) == NULL){
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "%s%d\n", MIN_GRADE_FILE_TEMPLATE, neccesary_grade);
    fclose(fp);
    free(pathname);
}