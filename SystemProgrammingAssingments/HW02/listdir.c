/*
 * HW02_121044029_Mehmet_Gurol_Cay
 *
 * File: main.c
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

#define ERROR_CODE -1
#define NEW_LINE '\n'       // NEW LINE
#define WHITE_SPACE ' '     // WHITE SPACE
#define TAB '\t'            // TAB SPACE
#define LOG_FILE_NAME "log.txt"
#define BUF_SIZE 1024

void startSearch(char *fileName,const char *searchedWord);
void readFromFile(FILE *pFileInput,char *fileContent,int longestLineNum);
void searchInFileContent(const char *fileContent, int **rowLineTable, int rowNumber,
                         const char *searchedWord, char filePath[BUF_SIZE]);
void printFoundWordLocation(int **rowLineTable, int foundedColumn,
                            int rowNumber,const char *searchedWord,
                            FILE *pFileOutput,char *fileName);
int fillToRowLineTable(FILE *pFileInput,int rowNumber,int **rowLineTable);
int countRowNumber(FILE *pFileInput);
int traversePath(const char *dirPath, char filePaths[][BUF_SIZE]);
void countLogFileLineNumber();

int static totalPathNum = 0;

int main(int argc, char const *argv[]) {
    char filePaths[BUF_SIZE][BUF_SIZE];
    char dirPath[BUF_SIZE];
	int i = 0, totalFoundWordNum = 0;
    pid_t childpid;

    if (argc != 3) {
        fprintf(stdout, "Usage: %s \"string\" <FullDirPath>\n",argv[0] );
        return 1;
    }

    memset(dirPath,'\0',BUF_SIZE);

    if(getcwd(dirPath, sizeof(dirPath)) == NULL){
        perror("getcwd() error");
        return 0;
    }
    strcat(dirPath,"/");
    strcat(dirPath,argv[2]);

    if (ERROR_CODE == traversePath(dirPath,filePaths))
		return 1;

	for (i = 0; i < totalPathNum; ++i){
		if((childpid = fork()) <= 0)
			break;
	}

	if(childpid == 0){
		startSearch(filePaths[i],argv[1]); //argv[1]: searchedWord
		exit(1);
	} else if(childpid == -1){
		perror("Error in fork. Program will be terminated.");
		exit(1);
	} else {
		while( wait(NULL) != -1);
        countLogFileLineNumber();
	}
}

/**
* Toplam bulunan kelime sayisini bulur ve bunu log.txt dosyasina basar
*
*/
void countLogFileLineNumber(){
    FILE *pFileOutput;
    int counter = 0;
    char temp;
    pFileOutput = fopen(LOG_FILE_NAME,"a+");

    while (!feof(pFileOutput)){
        temp = fgetc(pFileOutput);
        if (temp == NEW_LINE) {
            ++counter;
        }
    }
    fprintf(pFileOutput,"Total word in the searched: %d\n",counter);
    fclose(pFileOutput);
}


/*
 * Reference: https://github.com/bias/usp_exercises/blob/master/ch05/traversing-directories/mydu.c
 */
int traversePath(const char *dirPath, char filePaths[][BUF_SIZE]){
	char buf[BUF_SIZE];
	struct stat statbuf;
	struct dirent *direntp;
	DIR *pDir;

	if ((pDir = opendir(dirPath)) == NULL){
		fprintf(stderr, "%s path can not open! Program will be terminated\n",
				dirPath );
		return ERROR_CODE;
	}
	chdir(dirPath);
	while ((direntp = readdir(pDir)) != NULL) {
		if ( strcmp(direntp->d_name, ".") != 0 &&
			strcmp(direntp->d_name, "..") != 0 ) {
			strcat(strcat(strcpy(buf, dirPath), "/"), direntp->d_name);

			if ( lstat(buf, &statbuf) == -1) {
				perror("Couldn't get file status");
				fprintf(stderr, "\t%s/%s\n", dirPath, direntp->d_name);
				return ERROR_CODE;
			}
			if ( S_ISDIR(statbuf.st_mode) ) { // dir
				traversePath(buf,filePaths);
			}
			else if ( S_ISREG(statbuf.st_mode) ) { // files
				strcpy(filePaths[totalPathNum],buf);
				++totalPathNum;
			}
        }
	}
	chdir("../");
	while ((closedir(pDir) == -1) && (errno == EINTR));

	return 0;
}
/**
 * This function is drive function. All operations are applied in this function.
 *
 * @param	filePath		this file path is founded
 * @param 	*searchedWord	this word is wanted word in the file
 */
void startSearch(char filePath[BUF_SIZE],const char *searchedWord){
    FILE *pFileInput;

    int rowNumber,i = 0, **rowLineTable, allCharNum = 0, longestLineNum = 0;
    char *fileContent;


    pFileInput = fopen(filePath,"r");

    if (pFileInput == NULL) {
        fprintf(stderr, "%s could not be opened!\n",filePath );
        exit(0);
    }

    rowNumber = countRowNumber(pFileInput);

    rowLineTable = (int **) calloc(sizeof(int *) , (rowNumber+1));


    if (rowLineTable == NULL) {
        fprintf(stderr, "rowLineTable is NULL\n" );
        exit(1);
    }

    for (i = 0; i < rowNumber; i++) {
        rowLineTable[i] = (int *) calloc(sizeof(int) , 2);
        if (rowLineTable[i] == NULL) {
            exit(1);
        }
    }

    rewind(pFileInput);

    longestLineNum = fillToRowLineTable(pFileInput,rowNumber,rowLineTable);

    rewind(pFileInput);

    allCharNum = rowLineTable[rowNumber-1][1];

    fileContent = (char *) calloc(sizeof(char) , (allCharNum+1));


    readFromFile(pFileInput,fileContent,longestLineNum);

    searchInFileContent(fileContent,rowLineTable,rowNumber,searchedWord,filePath);

    free(fileContent);
    for (i = 0; i < rowNumber; i++) {
        free(rowLineTable[i]);
    }

    free(rowLineTable);
    fclose(pFileInput);
}

/**
* Dosya icinde aranacak kelimeyi arar
*
* @param    fileContent     dosya icerigini tek bir char pointerda tutulur
* @param    rowLineTable    her satirin kac karakterden olustugunu tutan tablo
* @param    rowNumber       dosya satir sayisi
* @param    searchedWord    dosyalar icinde aranacak kelime
* @param    filePath        icerigi aranacak dosyanin full path'i
*/
void searchInFileContent(const char *fileContent, int **rowLineTable, int rowNumber,
                         const char *searchedWord,
                            char filePath[BUF_SIZE]){
    FILE *pFileOutput;
    int i = 0,j = 0, k = 0, m = 0;
    char *temp, *fileName, *fileNameTemp;
    int allChar = rowLineTable[rowNumber-1][1];

    temp = (char *)calloc((strlen(searchedWord) + 1) , sizeof(char));

    pFileOutput = fopen(LOG_FILE_NAME,"a+");

    fileNameTemp = strtok(filePath,"/");
    while (fileNameTemp != NULL) {
        fileNameTemp = strtok(NULL,"/");
        if (fileNameTemp != NULL) {
            fileName = fileNameTemp;
        }
    }

    for (i = 0; i < allChar; i++) {
        for (j = i, k = 0; k < strlen(searchedWord) &&  j < allChar; j++) {
            if(fileContent[j] != NEW_LINE && fileContent[j] != WHITE_SPACE && fileContent[j] != TAB){
                temp[k] = fileContent[j];
                ++k;
            }
            if (k == 0 &&
                (fileContent[j] == NEW_LINE ||
                 fileContent[j] == WHITE_SPACE ||
                 fileContent[j] == TAB)) {
                break;
            }
        }
        temp[k] = '\0';
        if (strcmp(temp,searchedWord) == 0) {
            for(m = 0; m < strlen(searchedWord); ++m){
                temp[m] = '\0';
            }
            printFoundWordLocation(rowLineTable,i+1,rowNumber,searchedWord,pFileOutput,fileName);
        }
    }
    fclose(pFileOutput);
    free(temp);
}
/**
* Bulunan kelimenin dosya icinde bulundugu zaman hangi kolon ve satirda oldugunu
* bulup log dosyasina basar
*
* @param rowLineTable       her satirin kac karakterden olustugunu tutan tablo
* @param foundedColumn      kelimenin bulundugu baslangic sutun sayisi
* @param rowNumber          dosya satir sayisi
* @param searchedWord       dosyalar icinde aranacak kelime
* @param pFileOutput        log.txt dosyasi
* @param fileName           dosya ismi
*/
void printFoundWordLocation(int **rowLineTable, int foundedColumn,
                            int rowNumber,const char *searchedWord,
                            FILE *pFileOutput,char *fileName) {
    int i = 0;

    if (pFileOutput == NULL) {
        perror("Log file could not be opened!" );
        exit(0);
    }
    for (i = 0; i < rowNumber; i++) {
        if(foundedColumn < rowLineTable[i][1]){
            if(i == 0){
                fprintf(pFileOutput, "%s:[%d,%d] first character is found\n",
                        fileName,rowLineTable[i][0],foundedColumn);
            }else{
                fprintf(pFileOutput, "%s:[%d,%d] first character is found\n",
                        fileName,rowLineTable[i][0],(foundedColumn - rowLineTable[i-1][1]));
            }
            break;
        }
    }
}
/**
* Tum dosya icerigini fileContent'e doldurur
*
* @param    pFileInput      icerigi arastiralacak dosya pointer'i
* @param    fileContent     dosya icerigini tek bir char pointerda tutulur
* @param    longestLineNum  dosya icindeki en uzun satir
*/
void readFromFile(FILE *pFileInput,char *fileContent,int longestLineNum) {
    char *line = NULL;

    line = (char *)calloc(sizeof(char) , (longestLineNum+1));

    while(!feof(pFileInput)){
        if(fgets(line,longestLineNum,pFileInput) == NULL)
            break;
        //fprintf(stderr, "%s",line );
        strcat(fileContent,line);
    }

    free(line);
}
/**
* rowLineTable tablosunu satirlariyla iliski olarak hangi satir kac karakterden
* olustugunu kaydeder
*
* @param    pFileInput      icerigi arastiralacak dosya pointer'i
* @param    rowNumber       dosya satir sayisi
* @param    rowLineTable    her satirin kac karakterden olustugunu tutan tablo
* @return   dosya icindeki en uzun satirin karakter sayisini dondurur
*/
int fillToRowLineTable(FILE *pFileInput,int rowNumber,int **rowLineTable) {
    int i = 0,countOfChar = 1, longestLineNum = 0;
    char buf;

    while (!feof(pFileInput)) {
        buf = fgetc(pFileInput);

        if (buf == NEW_LINE) {
            if(countOfChar > longestLineNum){
                longestLineNum = countOfChar;
            }

            rowLineTable[i][0] = i + 1;
            rowLineTable[i][1] = countOfChar;
            ++i;
        }
        ++countOfChar;
    }

    return longestLineNum;
}
/**
* Input dosyasinin kac satirdan olustugunu bulur. Bu bulunan deger rowLineTable
* icinde tutulur
* @param    pFileInput      icerigi arastiralacak dosya pointer'i
*/
int countRowNumber(FILE *pFileInput) {
    char buf;
    int rowNumber = 0;

    while (!feof(pFileInput)) {
        buf = fgetc(pFileInput);
        if (buf == EOF) {
            break;
        }

        if (buf == NEW_LINE) {
            ++rowNumber;
        }
    }

    return rowNumber;
}
