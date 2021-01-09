CC = gcc
DB = gdb
CFLAG1 = -c
CFLAG2 = -o
DFLAG = -g
PROGRAM_NAME = newListDir

all:
	$(CC) $(CFLAG1) $(PROGRAM_NAME).c
	$(CC) $(PROGRAM_NAME).o $(CFLAG2) exe

debug:
	$(CC) $(DFLAG) $(PROGRAM_NAME).c $(CFLAG2) exe
	$(DB) ./exe

run:
	$(CC) $(CFLAG1) $(PROGRAM_NAME).c
	$(CC) $(PROGRAM_NAME).o $(CFLAG2) exe
	./exe ece test
valgrind:
	valgrind -v --tool=memcheck --leak-check=yes --show-reachable=yes ./exe ece test

clean:
	rm -f $(PROGRAM_NAME) *.o exe log.txt *.fifo
