//
// Created by gurol on 31.05.2018.
//

#ifndef FINAL_MAIN_H
#define FINAL_MAIN_H

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <signal.h>
#include <unistd.h>
#include <net/if.h>
#include <stdint.h>
#include <stdbool.h>
#include <ifaddrs.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

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
#define BUFFSIZE        256
#define MAXQUEUENUM     2
#define PI              3.141592654
//// New data structure definition
struct Client {
	char priority;
	int degree;
	char clientName[MINSTRSIZE];
	int clientSocket;
	int connectionTime;
};

struct Provider {
	char providerName[MINSTRSIZE];
	int performance;
	int price;
	int duration;
	pthread_t threadID;
	int front;
	int rear;
	int countQueue;
	struct Client providerQueue[2];
	pthread_cond_t condVarProvider;
	pthread_mutex_t condVarLocProvider;
	bool isProviderRunning;
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
struct Provider *providers = NULL;
FILE *pLogFile;
char *serverIPAddr;
int providerNum = 0;
bool isRunning = true;
int serverSock;
int clientSock;
struct sockaddr_in serverAddress;
struct sockaddr_in clientAddress;
unsigned short serverSocketNumber;
pthread_t *providersThID;
pthread_t connectionHandleThID;
pthread_t durationTimeCheckerThID;

int aliveManagerTh = 0;
int aliveProviderTh = 0;

pthread_mutex_t condVarLocTh = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condVar = PTHREAD_COND_INITIALIZER;
pthread_mutex_t condVarLoc = PTHREAD_MUTEX_INITIALIZER;
//// End of global variable definition


//// Function definition

void parseDataFile(const char *fileName);

void *ThreadProvider(void *id);

void *ThreadManager(void *clientSock);

void *ThreadConnectionHandler(void *var);

void *ThreadDurationTimeChecker(void *var);

int startServer();

void selectDevice();

int acceptClient();

long int factorial(int n);

double calcCosDeg(int degree);

long int factorial(int n);

struct Client peek(int providerNum);

bool isEmpty(int providerNum);

bool isFull(int providerNum);

int size(int providerNum);

void insert(int providerNum, struct Client newClient);

struct Client removeData(int providerNum);

void signalHandler(int signum);
//// End of function definition



#endif //FINAL_MAIN_H
