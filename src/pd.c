/*Code written by Francisco Jesús Díaz Pellejero
  Email: FcoJesus.Diaz@alu.uclm.es
  Github repository source code: https://github.com/FcoJesusDiaz/SSOO-p1.git
  ---------------------------------------------------------------------------------
  Code related to the pd process. For the recursive removal of the students directory
  I could have used the system() function but I prefered to use opendir(), readdir(), etc
  because it is less resource heavy.*/


/*HEADERS*/
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "definitions.h"

/*DECLARATION OF FUNCTIONS*/
int remove_directory(char *path);


/*MAIN*/
int main(){
    char directory[] = STUDENTS_DIR;
    directory[strlen(STUDENTS_DIR) - 1] = '\0'; /*Delete the character '/' from "estudiantes/"*/
    
    if(!remove_directory(directory)) return EXIT_SUCCESS;
    fprintf(stderr, "No se ha podido borrar el directorio de estudiantes\n");
    return EXIT_FAILURE;
}

/*DEFINITION OF FUNCTIONS*/
int remove_directory(char *path) {
    DIR *dir_stream = opendir(path);
    struct dirent *file;
    char *buf;
    struct stat statbuf;
    size_t filename_len;
    size_t path_len = strlen(path);
    int result = 0;

    if (!dir_stream) {
        fprintf(stderr, "%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    while (!result && (file = readdir(dir_stream))) {

        /* Skip the names "." and ".." as we don't want to recurse on them. */
        if (!strcmp(file->d_name, ".") || !strcmp(file->d_name, ".."))
        continue;

        result = -1;

        filename_len = path_len + strlen(file->d_name) + 2; 
        buf = malloc(filename_len);
        
        snprintf(buf, filename_len, "%s/%s", path, file->d_name);
    
        /*I used lstat because the models inside "estudiantes/<dni>/<model> are
        symbolic links to "examenes/<model>.pdf""*/
        if (!lstat(buf, &statbuf)) {
            if (S_ISDIR(statbuf.st_mode))
                result = remove_directory(buf);
            else
                result = unlink(buf);
        }else{
            fprintf(stderr, "%s\n", strerror(errno));
        }

        free(buf);
        
    }
    closedir(dir_stream);

    if (!result) result = rmdir(path);
    return result;
}