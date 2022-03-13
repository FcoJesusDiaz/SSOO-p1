/*Code written by Francisco Jesús Díaz Pellejero
  Email: FcoJesus.Diaz@alu.uclm.es
  Github repository source code: https://github.com/FcoJesusDiaz/SSOO-p1.git
  ---------------------------------------------------------------------------------
  Code related to the backup process. For the recursive copying of the entire project
  I could have used the system() function but i prefered to use opendir(), readdir(), etc
  because it is less resource heavy.*/


/*HEADERS*/
#define _POSIX_C_SOURCE 200809L /*Needed for the snprintf function*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

#include "definitions.h"

/*DECLARATION OF FUNCTIONS*/
int backup_dir(char *target, char *source);

/*MAIN*/
int main(int argc, char* argv[]){
    pid_t process_id = 0;
    pid_t sid = 0;

    process_id = fork();
    if (process_id < 0){
        exit(EXIT_FAILURE);
    }

    // Kill parent process --> child runs on background
    if (process_id > 0){
        exit(EXIT_SUCCESS);
    }

    // Unmask the file mode
    umask(0);

    // Set new session --> Detach from a controlling terminal
    // This call fails if the calling process is a group leader.
    // We are in the child process so this call will work.
    sid = setsid();

    if(sid < 0){
        exit(EXIT_FAILURE);
    }

    // Change the current working directory to parent.
    chdir("..");

    // Close stdin, stdout and stderr
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1){
        /*Creates backup directory if it does not exists*/
        if(!opendir(BACKUP_DIR)) mkdir(BACKUP_DIR, 0770);

        backup_dir(PROJECT_NAME, BACKUP_DIR);
        sleep(60);
    }
}

/*DEFINITION OF FUNCTIONS*/
int backup_dir(char *target, char *source){
    DIR *dir_stream = opendir(target);
    struct dirent *file;
    size_t source_len = strlen(source);
    size_t target_len = strlen(target);
    char *source_file; /*File inside source directory*/
    char *target_file; /*File inside target directory*/
    size_t source_file_len;
    size_t target_file_len;
    struct stat statbuf;
    int result = 0;

    if (!dir_stream) {
        fprintf(stderr, "%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    while (!result && (file = readdir(dir_stream))) {
        /* Skip the names "." and ".." as we don't want to recurse on them. 
        I also skipped executables files and object files.*/
        if (!strcmp(file->d_name, ".") || !strcmp(file->d_name, "..") || !strcmp(file->d_name, EXEC_DIR)
        || !strcmp(file->d_name, OBJ_DIR) || !strcmp(file->d_name, DEBUG_DIR))
        continue;

        result = -1;

        source_file_len = source_len + strlen(file->d_name) + 2; 
        source_file = malloc(source_file_len);

        target_file_len = target_len + strlen(file->d_name) + 2;
        target_file = malloc(target_file_len);

        snprintf(source_file, source_file_len, "%s/%s", source, file->d_name);
        snprintf(target_file, target_file_len, "%s/%s", target, file->d_name);
    
        if (!lstat(target_file, &statbuf)) {
            if (S_ISDIR(statbuf.st_mode)){
                /*Creates directory if it does not exists*/
                if(!opendir(source_file)){
                    mkdir(source_file, 0770);
                } 
                result = backup_dir(target_file, source_file);
            }
            else if(stat(source_file, &statbuf))
                result = link(target_file, source_file);
            else result = 0;
                
        }else{
            fprintf(stderr, "%s\n", strerror(errno));
        }
        free(source_file);
        free(target_file);
        
    }
    closedir(dir_stream);

    return result;
}