# SSOO-p1
## STRUCTURE OF THE PROJECT
- src --> All the source code of the project. The manager, pa, pb, pc, pd and backup processes.
- incude --> It includes the definitions.h header with all the macros used by all the .c files.
- examenes --> It conatins the 3 models of the second exam.
- estudiantes.txt --> File containing the DNI, the exam model that the student has to take and the grade of the first exam.
- Makefile --> It contains all the rules for the compulation of the source code

## MAKEFILE RULES
- dirs --> It will create the object, executable and debugging directories.
- manager --> It will create the manager executable.
- pa --> It will create the pa executable.
- pb --> It will create the pb executable.
- pc --> It will create the pc executable.
- pd --> It will create the pd executable.
- backup --> It will create the backup executable daemon.
- daemon --> It will execute the backup daemon that will run periodically. Commands have to be used to end the process.
- rm_student --> It will execute the pd process to delete the student directory.
- debugging --> It will create the debug directory with all the executables that are ready to be debugged with gdb.
- all --> It includes the dirs, manager, pa, pb, pc, pd and backup rules.
- clean --> It will delete the object, executable and debugging directories.
- test --> It will execute the manager process.
