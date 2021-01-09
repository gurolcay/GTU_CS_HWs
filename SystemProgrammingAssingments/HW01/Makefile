CC = gcc
DB = gdb
CFLAG1 = -c
CFLAG2 = -o
DFLAG = -g
PROGRAM_NAME = main

all:
	$(CC) $(CFLAG1) $(PROGRAM_NAME).c
	$(CC) $(PROGRAM_NAME).o $(CFLAG2) $(PROGRAM_NAME)

debug:
	$(CC) $(DFLAG) $(PROGRAM_NAME).c $(CFLAGS) $(PROGRAM_NAME)
	$(DB) ./$(PROGRAM_NAME)

run:
	./$(PROGRAM_NAME) oto test.txt

clean:
	rm -f $(PROGRAM_NAME) *.o main
