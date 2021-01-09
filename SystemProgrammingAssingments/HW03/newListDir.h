/*
 * HW03_121044029_Mehmet_Gurol_Cay
 *
 * File: listdir.h
 * Author: Mehmet Gürol Çay
 *
 * Created on March 7, 2017, 07:45 PM
 *
 * Description
 * -----------
 * This program searches a word in the given directory. This program write the
 * log file that is called log.txt.
 * Ignored character are 'tab', '\n' and white space.
 *
 */

#ifndef LISTDIR_H
#define LISTDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define ERROR -1
#define SUCCESS 1
#define NEW_LINE '\n'       // NEW LINE
#define WHITE_SPACE ' '     // WHITE SPACE
#define TAB '\t'            // TAB SPACE
#define LOG_FILE_NAME "log.txt"
#define BUF_SIZE 1024
#define MAX_FILE_NAME 255
#define DIR_STTS 1
#define FILE_STTS 2
// #define FIFO_PERMS (S_IRUSR | S_IWUSR| S_IRGRP | S_IROTH)
#define FIFO_PERMS (S_IRWXU | S_IWGRP | S_IWOTH)

//if filePath represents a file fileStatus set up as FILE_STTS
//if filePath repsesents a directory fileStatus set up as DIR_STTS
typedef struct {
    int fileStatus;
    char *filePath;
    int pipeFD[2];
}fileInf_t;

ssize_t r_write(int fd, void *buf, size_t size);
void wrapper(const char *dirPath);
void startSystem(const char *dirPath);
int getDirAndFileNum(const char *dirPath,int *dirNum, int *fileNum);
void startSearch(char filePath[BUF_SIZE],int wPipeFD);
void searchInFileContent(const char *fileContent, int **rowLineTable, int rowNumber,
                            char filePath[BUF_SIZE],int wPipeFD);
void printFoundWordLocation(int **rowLineTable, int foundedColumn,
                            int rowNumber,FILE *pFileOutput,char *fileName,
                            int wPipeFD);
void readFromFile(FILE *pFileInput,char *fileContent,int longestLineNum);
int fillToRowLineTable(FILE *pFileInput,int rowNumber,int **rowLineTable);
int countRowNumber(FILE *pFileInput) ;
void countLogFileLineNumber();

static char *searchedWord;
char mainFifoName[MAX_FILE_NAME] = "main.fifo";

#endif
