/*
 * Midterm_121044029_Mehmet_Gurol_Cay
 *
 * timerServer.h
 * -------
 * Created on 09/04/2016 by Mehmet Gurol Cay
 *
 * Description
 * -----------
 * 		The timerServer program is a server. It meets requests from the seeWhat
 *      program. This program make a fork when each SeeWhat program to
 *      connection. The timerServer program generates randomly invertible matrix and
 *      sends it to SeeWhat program.
 *
 */

#ifndef TIMESERVER_H
#define TIMESERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>

#define ERROR -1
#define FIFO_PERMS (S_IRWXU | S_IWGRP | S_IWOTH)
#define MM_SIZE 20  // maximum matrix size
#define BILLION 1000000000L
#define MILLION 1000000L
#define TIMER_MSG "\nReceived Timer Interrupt\n"
#define SERVER_LOG_NAME "./log/timerServerLog"

typedef enum{
	false , true
} bool;

//Global variables
struct ServerInf {
	int serverPID;
	int matrixSize;
	int timeMSec;
};

struct SubServerInf{
	double matrix[MM_SIZE][MM_SIZE];
	double det;
	int subServerPID;
	int isKill;
};

struct ClientInf {
	int clientPID;
	char clientFifoName[32];
};

struct ServerLog {
	int clientPID;
	int matrixSize;
	double det;
	char matrixGenerationTime[32];
};

char *mainFifoName,
	clientFifoName[32];
int	timeMiliSec = 0;
int mainFifoFD,
	clientFifoFD;
FILE *pServerLog;

int clientFlag = 0;
int sigIntFlag = 0;
int requestFlag = 0;

struct ServerInf serverInf;
struct SubServerInf subServerInf;
struct ClientInf clientInf;
struct ServerLog serverLog;
struct timespec startServerTime;
struct timespec clientConnectionTime;


void generateMatrix(double matrix[][MM_SIZE],int matrixSize);
bool isInvertibleMatrix();
double **generateUnitMatrix(int matrixSize);
double **allocMatrix(int size);
void freeMatrix(double **matrix, int matrixSize);
void SIGINT_handler(int signum);
void SIGUSR1_handler(int signum);
void SIGUSR2_handler(int signum);
void startServer(struct ServerInf svr, struct SubServerInf subSvr);
bool createServerPidFile();
double determinant(double **mtrx,int size);
void alarmHandler();
bool checkServerInput(const char *ms, const char *n);
void readFromClient();
ssize_t r_read(int fd, void *buf, size_t size);
void writeToLog();
void copyMatrix(double **destMtrx, int size);

struct stat st = {0};

#endif
