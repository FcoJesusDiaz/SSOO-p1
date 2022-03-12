DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/
DIRDEBUG := debug/

CFLAGS := -I$(DIRHEA) -c -Wall -std=c99
LDLIBS := -lpthread -lrt
CC := gcc

all : dirs manager pa pb pc pd

dirs:
	mkdir -p $(DIROBJ) $(DIREXE)

debugging:
	$(CC) -I$(DIRHEA) -g $(DIRSRC)manager.c -o $(DIRDEBUG)manager $(LDLIBS)
	$(CC) -I$(DIRHEA) -g $(DIRSRC)pa.c -o $(DIRDEBUG)pa $(LDLIBS)
	$(CC) -I$(DIRHEA) -g $(DIRSRC)pb.c -o $(DIRDEBUG)pb $(LDLIBS)
	$(CC) -I$(DIRHEA) -g $(DIRSRC)pc.c -o $(DIRDEBUG)pc $(LDLIBS)
	$(CC) -I$(DIRHEA) -g $(DIRSRC)pd.c -o $(DIRDEBUG)pd $(LDLIBS)

manager: $(DIROBJ)manager.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

pa: $(DIROBJ)pa.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

pb: $(DIROBJ)pb.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

pc: $(DIROBJ)pc.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

pd: $(DIROBJ)pd.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

$(DIROBJ)%.o: $(DIRSRC)%.c
	$(CC) $(CFLAGS) $^ -o $@

test:
	./$(DIREXE)manager

solution:
	./$(DIREXE)manager

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRDEBUG) $(DIRHEA)*~ $(DIRSRC)*~
	