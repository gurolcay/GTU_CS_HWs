CC = gcc
DB = gdb
CFLAG1 = -c
CFLAG2 = -o
DFLAG = -g
PROGRAM_NAME = listdir

all:
	$(CC) $(CFLAG1) $(PROGRAM_NAME).c
	$(CC) $(PROGRAM_NAME).o $(CFLAG2) exe

debug:
	$(CC) $(DFLAG) $(PROGRAM_NAME).c $(CFLAG2) exe
	$(DB) ./exe

run:
	./exe alp mydir

clean:
	rm -f $(PROGRAM_NAME) *.o exe log.txt
