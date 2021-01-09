CC = gcc
DB = gdb
CFLAG1 = -c
CFLAG2 = -o
DFLAG = -g
THFLAG = -lpthread
PROGRAM_NAME = grepTh

all:
	$(CC) $(CFLAG1) $(PROGRAM_NAME).c
	$(CC) $(PROGRAM_NAME).o $(THFLAG)  $(CFLAG2) exe

debug:
	$(CC) $(DFLAG) $(PROGRAM_NAME).c $(THFLAG) $(CFLAG2) exe
	$(DB) ./exe

run:
	./exe alp mydir

valgrind:
	valgrind -v --log-file=LOGVAL.txt --tool=memcheck  --leak-check=yes --show-reachable=yes  ./exe test complexFolder

clean:
	rm -f $(PROGRAM_NAME) *.o exe log.txt
