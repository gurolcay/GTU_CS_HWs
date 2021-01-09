all:
	gcc -std=gnu11 121044029_main.c -o exe
	gcc -std=gnu11 ls.c -o ls
	gcc -std=gnu11 wc.c -o wc

valgrind:
	valgrind -v --log-file=LOGVAL.txt --tool=memcheck  --leak-check=yes --show-reachable=yes --track-origins=yes ./exe

debug:
	gcc -g -std=gnu11 -Wall 121044029_main.c -o exe
