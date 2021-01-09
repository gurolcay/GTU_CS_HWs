all:
	gcc -std=gnu11 121044029_main.c -pthread -lrt -lm -o  exe

valgrind:
	valgrind -v --log-file=LOGVAL.txt --tool=memcheck  --leak-check=yes --show-reachable=yes --track-origins=yes ./exe data.dat

debug:
	gcc -g -std=gnu11 -Wall 121044029_main.c -pthread -lrt -lm -DDEBUG=1 -o exe

clean:
	rm -r LOGVAL.txt exe
