//
// Created by gurol on 31.05.2018.
//

#ifndef FINAL_CLIENT_H
#define FINAL_CLIENT_H

#include <time.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <semaphore.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

//// Color definition for terminal outputs
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"
//// end of color definition

#define MINSTRSIZE      32


//// New data structure definition
struct Client {
	char priority;
	int degree;
	char clientName[MINSTRSIZE];
	int clientSocket;
	double result;
};

struct Result {
	char providerName[MINSTRSIZE];
	double taskCompTime;
	double result;
	int cost;
	bool error;
	bool shutdown;
};
//// End of new data structure definition

//// Global variable definition
struct sockaddr_in serverAddr;
int serverPortNumber;
int serverSock;
char serverIP[MINSTRSIZE];
//// End of global variable definition

//// Function definition
int createClientSocket();

int connectToServer();
//// End of function definition
#endif //FINAL_CLIENT_H
