CC = gcc
DB = gdb
CFLAGS = -c
OFLAGS = -o
DFLAGS = -g
SERVERPROGNAME = timerServer
CLIENTPROGNAME = seeWhat
RESPROGNAME = showResults
LINKMATH_H = -lm

all:
	$(CC) $(CFLAGS) $(SERVERPROGNAME).c
	$(CC) $(SERVERPROGNAME).o $(LINKMATH_H) -lrt $(OFLAGS)  $(SERVERPROGNAME)

	$(CC) $(CFLAGS)  $(CLIENTPROGNAME).c
	$(CC) $(CLIENTPROGNAME).o $(LINKMATH_H) $(OFLAGS) $(CLIENTPROGNAME)

	$(CC) $(CFLAGS)  $(RESPROGNAME).c
	$(CC) $(RESPROGNAME).o $(OFLAGS) $(RESPROGNAME)

debugServer:
	$(CC) $(DFLAGS) $(SERVERPROGNAME).c $(CFLAGS) $(LINKMATH_H) $(SERVERPROGNAME)
	$(DB) ./$(SERVERPROGNAME)

debugClient:
	$(CC) $(DFLAGS) $(CLIENTPROGNAME).c $(CFLAGS) $(CLIENTPROGNAME)
	$(DB) ./$(CLIENTPROGNAME)

debugRes:
	$(CC) $(DFLAGS) $(RESPROGNAME).c $(CFLAGS) $(RESPROGNAME)
	$(DB) ./$(RESPROGNAME)

runServer:
	./$(SERVERPROGNAME) 5 7 mainfifo.fifo

runClient:
	./$(CLIENTPROGNAME) mainfifo.fifo

runResult:
	./$(RESPROGNAME)

valgrindServer:
	valgrind -v --tool=memcheck --leak-check=yes --trace-children=yes --show-reachable=yes ./$(SERVERPROGNAME) 5 2 mainfifo.fifo

valgrindClient:
	valgrind -v --tool=memcheck --leak-check=yes --show-reachable=yes ./$(CLIENTPROGNAME) mainfifo.fifo

valgrindResult:
	valgrind -v --tool=memcheck --leak-check=yes --show-reachable=yes ./$(RESPROGNAME)

clean:
	rm -f $(SERVERPROGNAME) $(CLIENTPROGNAME) $(RESPROGNAME) *.o *.log *.fifo
