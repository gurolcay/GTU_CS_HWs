/*																			  
 * HW02_121044029_Mehmet_Gurol_Cay											  
 *																			  
 * main.c																	  
 * -------																	  
 * Created on 20/03/2016 by Mehmet Gürol Çay								  
 *																			  
 * Description																  
 * -----------																  
 * This program searches a word in the given directory. 
 * When this program is searching a word, program is creating a fork for each
 * files and directories.
 * This program give to output a log file. Log file name is gfd.log										  
 *																			  
 * Notes																	  
 * -----------																  
 * 		I will explain functions on the every function header.				  
 *																			  
 *		Please set the tap size: 4											  
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

#define ERROR_CODE -1
#define DEF_SIZE 100
#define BUF_SIZE 1024

typedef enum {false, true} bool;

int countLongestLine(FILE *inputFile);
void readFromFile (FILE *inputFile,int longestWordCharNum,char *line);
int searchAWord (const char *searchedWord, const char *aLine,int *lineNumber,
					FILE *outputFile);
void searchInFile(const char *filePath, const char *searchedWord);
int traversePath(const char *dirPath, char filePaths[][BUF_SIZE]);

int static totalPathNum = 0;
int static totalFoundWordNum = 0;

int main(int argc, char const *argv[])
{
	char filePaths[BUF_SIZE][BUF_SIZE];
	int i = 0;
	pid_t childpid;

	if (argc != 3){
		fprintf(stderr,"Usage: %s <FullDirPath> \"string\" \n",argv[0]);
		return 1;
	}
	
	if (ERROR_CODE == traversePath(argv[1],filePaths))
		return 1;

	for (i = 0; i < totalPathNum; ++i){
		if((childpid = fork()) <= 0)
			break;
	}


	if(childpid == 0){
		searchInFile(filePaths[i],argv[2]);
		exit(1);	
	} else if(childpid == -1){
		perror("Error in fork. Program will be terminated.");
		exit(1);
	} else {
		while( wait(NULL) != -1); 
	
		printf("Total word in the search: %d\n", totalFoundWordNum);
	}




	return 0;
}

/*
 * Reference: https://github.com/bias/usp_exercises/blob/master/ch05/traversing-directories/mydu.c
 */
int traversePath(const char *dirPath, char filePaths[][BUF_SIZE]){
	char buf[1024];
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
				fprintf(stderr, "\t%s %s\n", dirPath, direntp->d_name);
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
 * This function finds longest line in the file
 *
 * @param   *inputFile	input file pointer	
 * @return 				longest line character number
 */
int countLongestLine(FILE *inputFile){
	
	int bytesToRead = 0;
	int numberOfLongestLine = 0;
	int longestWordCharNum = 0;
	char buf;

	while(!feof(inputFile)){
		
		buf = fgetc(inputFile);

		if (buf == EOF)
			break;

		if (buf == '\n')
			bytesToRead = 0;
		
		++bytesToRead;

		if (bytesToRead > longestWordCharNum)
			longestWordCharNum = bytesToRead;	
	}


	return longestWordCharNum;
}
/**
 * This function reads a file line by line.
 *
 * @param	*inputFile			input file pointer
 * @param	longestWordCharNum	longest line charcter number
 * @param 	*line				this is a line from file.
 */
void readFromFile (FILE *inputFile,int longestWordCharNum,char *line){
	
	int i = 0;
	int bytesToRead = 0;

	fgets(line, longestWordCharNum + 1, inputFile);
}
/**
 * This function is finding a word in the line.
 *
 * @param	*searchedWord	this is word that is wanted word 
 * @param 	*aLine			this is a line from file
 * @param	*lineNumber		this is line number that is found word in which line
 * @param	*outputFile		output file pointer
 */
int searchAWord (const char *searchedWord, const char *aLine, int *lineNumber,
					FILE *outputFile){

	int i = 0,j = 0, k = 0,
		counter = 0;
	bool ifWordExist = false;
	char *temp;
	char logInf[DEF_SIZE];

	temp = (char *) malloc (sizeof(char) * strlen(searchedWord));

	for(k = 0; k < strlen(aLine); ++k){
		
		for (i = 0, j = k; i < strlen(searchedWord) && 
				j < strlen(aLine); ++i,++j)
			temp[i] = aLine[j];

		if(strcmp(temp,searchedWord) == 0) {
			++counter;
			if(ifWordExist == false){
				ifWordExist = true;
				fprintf(outputFile, "\n%s line: %d column: %d",
						searchedWord,*lineNumber, k + 1);
			}
		}
	}

	free(temp);
	
	return counter;
}
/**
 * This function is drive function. All operations are applied in this function.
 *
 * @param	fileName		this file name is given by user
 * @param 	*searchedWord	this word is wanted word in the file
 */
void searchInFile(const char filePath[BUF_SIZE], const char *searchedWord) {
	
	FILE *pFileInput;
	FILE *pFileOutput;

	int i = 0,
		bytesToRead = 0, 
		lineNumber = 1;
	char *aLine;
	


	pFileInput = fopen(filePath,"r");
	pFileOutput = fopen("gfF.log","a+");

	if ( pFileInput == NULL){
		printf("%s could not be opened!\n",*filePath );
		return;
	}

	if( pFileOutput == NULL) {
		printf("Output file can not be opened!\n");
		return;
	}

	bytesToRead = countLongestLine(pFileInput);

	rewind(pFileInput);

	aLine = (char *)malloc(sizeof(char) * bytesToRead);
	
	fprintf(pFileOutput, "%s",filePath );

	while(!feof(pFileInput)){
		readFromFile(pFileInput,bytesToRead,aLine);
		totalFoundWordNum += searchAWord(searchedWord,aLine,
										&lineNumber,pFileOutput);
		//fprintf(stderr, "totalFoundWordNum: %d\n",totalFoundWordNum);
		++lineNumber;
	}

	fprintf(pFileOutput, "\n" );

	free(aLine);

	fclose(pFileOutput);
	fclose(pFileInput);

	return;
}
