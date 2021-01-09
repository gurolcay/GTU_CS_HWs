#ifndef GREPTH_H
#define GREPTH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/msg.h>
#include <sys/syscall.h>
#include <semaphore.h>
#include <time.h>
#include <dirent.h>

#define ERROR -1
#define NEW_LINE '\n'       // NEW LINE
#define WHITE_SPACE ' '     // WHITE SPACE
#define TAB '\t'            // TAB SPACE
#define LOG_FILE_NAME "log.txt"
#define DEBUG_LOG_FILE "debugLog.txt"
#define BUF_SIZE 1024
#define BILLION 1000000000L
#define MILLION 1000000L
#define SEM_NAME "NamedSem"
#define PERMS (mode_t) (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define PERM (S_IRUSR | S_IWUSR)
#define FLAGS (O_CREAT | O_EXCL)

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

struct ThreadData{
    int processID;
    pthread_t thID;
    char filePath[BUF_SIZE];
    char searchedWord[BUF_SIZE];
    int pipeFD[2];
    int sharedID;
};

struct ProcessData{
    int maxThreadNumber;
    int searchedLineNumber;
    int searchedFileNumber;
    int liveThreadNum;
    int searcherThNum;
};

struct ThreadReturnData{
    int errorNo;
    int searchedLineNumber;
};

struct OutputData{
    int totalSearchedLineNum;
    int totalThreadNum;
    int totalSearchedDirNum;
    int totalSearchedFileNum;
    int totalCascadeThreadNum;
    int totalSearcherThreadNum;
    int totalRunTime;
};

void startProcess(char dirPath[BUF_SIZE],int wPipeFD,const char *searchedWord);
void *startThread(void *datas);
void readFromFile(FILE *pFileInput,char *fileContent,int longestLineNum);
void searchInFileContent(const char *fileContent, int **rowLineTable, int rowNumber,
                         const struct ThreadData *thData, char filePath[BUF_SIZE]);
void printFoundWordLocation(int **rowLineTable, int foundedColumn,
                            int rowNumber,const struct ThreadData *thData,
                            FILE *pFileOutput,char *fileName);
int fillToRowLineTable(FILE *pFileInput,int rowNumber,int **rowLineTable);
int countRowNumber(FILE *pFileInput);
int traversePath(const char *dirPath, char filePaths[][BUF_SIZE]);
void printToTerminal(struct OutputData outputData);
int getnamed(char *name, sem_t **sem, int val);
int detachandremove(int shmid, void *shmaddr);
void SIGINT_handler(int signum);

int static dirPathCounter = 0; //searched directory number
FILE *pFileDebug;
struct timespec firstTime;
struct timespec secondTime;
struct ThreadReturnData *thReturnDataG;
pthread_mutex_t muLock;
sem_t *semLockp;

int static liveThreadNum = 0;
int static searcherThNum = 0;
static int msqid;
key_t messageQueueKey;

#endif
