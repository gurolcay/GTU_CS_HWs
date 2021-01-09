CC = gcc
DB = gdb
CFLAGS = -o

all:
	$(CC) -c grepFromFile.c 
	$(CC) grepFromFile.o $(CFLAGS) grepFromFile

debug:
	$(CC) -g grepFromFile.c $(CFLAGS) grepFromFile
	$(DB) ./grepFromFile

run:
	./grepFromFile test.txt ece

clean:
	rm -f grepFromFile *.o gfF.log
