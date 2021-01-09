all:
	gcc -std=gnu11 server.c -pthread -lrt -lm -o  server
	gcc -std=gnu11 client.c -pthread -lrt -lm -o  client

valgrind_server:
	valgrind -v --log-file=LOGVAL.txt --tool=memcheck  --leak-check=yes --show-reachable=yes --track-origins=yes --show-leak-kinds=all ./server 5555 data.dat log.dat

valgrind_client:
	valgrind -v --log-file=LOGVAL.txt --tool=memcheck  --leak-check=yes --show-reachable=yes --track-origins=yes --show-leak-kinds=all ./client Hileci1 C 60 127.0.0.1 5555

debug:
	gcc -g -std=gnu11 -Wall server.c -pthread -lrt -lm -DDEBUG=1 -o server
	gcc -g -std=gnu11 -Wall client.c -pthread -lrt -lm -DDEBUG=1 -o client

clean:
	rm -r LOGVAL.txt server client

run_s:
	./server 5555 data.dat log.dat

run_c:
	./client Hileci1 C 60 127.0.0.1 5555
