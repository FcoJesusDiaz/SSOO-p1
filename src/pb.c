#include "definitions.h"

void copy_model(char *dni, char *model);

int main(int argc, char **argv){
    FILE *fptr = fopen(STUDENTS_INFO, "r");
    char *dni = NULL;
    char *model = NULL;
    char *lineptr = malloc(MAX_LINE);
    if(!(fptr && lineptr)){
        return EXIT_FAILURE;
    }
    while(fgets(lineptr, MAX_LINE, fptr)){
        if((dni = strtok(lineptr, " ")) && (model = strtok(NULL, " "))){
            copy_model(dni, model);
        }
    }
    free(lineptr);
    return EXIT_SUCCESS;
}

void copy_model(char *dni, char *model){
    int pdf_file_length = strlen("Modelo") + strlen(model) + strlen(".pdf") + 1;
    int target_name_length = strlen(EXAMS_DIR) + pdf_file_length;
    int linkpath_length = strlen(STUDENTS_DIR) + strlen(dni) + strlen("/") + pdf_file_length;

    char *target = malloc(target_name_length);
    char *linkpath = malloc(linkpath_length);

    snprintf(target, target_name_length,"%sModelo%s.pdf", EXAMS_DIR, model);
    snprintf(linkpath, linkpath_length, "%s%s/Modelo%s.pdf", STUDENTS_DIR, dni, model);
    
    symlink(target, linkpath);

    free(target);
    free(linkpath);
}
