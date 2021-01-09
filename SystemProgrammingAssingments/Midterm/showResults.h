/*
 * Midterm_121044029_Mehmet_Gurol_Cay
 *
 * showResults.h
 * -------
 * Created on 09/04/2016 by Mehmet Gurol Cay
 *
 * Description
 * -----------
 *      The showResults program shows the result of each seeWhat computution.
 *      The program generates log file which is included each seeWhat program
 *      matrix and results.
 *
 */


#ifndef SHOWRESULTS_H
#define SHOWRESULTS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define ERROR -1
#define SHOWRESULTS_FIFO "showResults.fifo"
#define SHOWRESULTS_LOG "./log/showResults.log"
#define FIFO_PERMS (S_IRWXU | S_IWGRP | S_IWOTH)

struct ResultOfMatrices{
	int clientPID;
    double orgDet;
	double detShifted;
	int calcTimeOfShifted;
	double detConvolution;
    int calcTimeOfConv;
    int isKill;
};


struct ResultOfMatrices resultOfMatrices;
FILE *pLogOutput;
int showResultFifoFD;

void writeToLog();
ssize_t r_read(int fd, void *buf, size_t size);
void SIGINT_handler(int signum);

#endif
