/*																			  
 * HW01_121044029_Mehmet_Gurol_Cay											  
 *																			  
 * main.c																	  
 * -------																	  
 * Created on 03/03/2016 by Mehmet Gürol Çay								  
 *																			  
 * Description																  
 * -----------																  
 * This program searches a word in the given file. This program give to output
 * a log file. Log file name is gfF.log										  
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

#define ERROR_CODE -1
#define DEF_SIZE 100

typedef enum {false, true} bool;

int countLongestLine(FILE *inputFile);
void readFromFile (FILE *inputFile,int longestWordCharNum,char *line);
int searchAWord (const char *searchedWord, const char *aLine,int *lineNumber,FILE *outputFile);
int searchInFile(const char *fileName, const char *searchedWord);

int main(int argc, char const *argv[])
{
	char *fileName;

	if (argc != 3){
		printf("Usage: grepfromFile.c <filename.xxx> \"string\" \n");
		return 1;
	}

	if( ERROR_CODE == searchInFile(argv[1],argv[2]))
		return 1;
	
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
int searchAWord (const char *searchedWord, const char *aLine,int *lineNumber,FILE *outputFile){

	int i = 0,j = 0, k = 0,
		counter = 0;
	bool ifWordExist = false;
	char *temp;
	char logInf[DEF_SIZE];

	temp = (char *) malloc (sizeof(char) * strlen(searchedWord));

	for(k = 0; k < strlen(aLine); ++k){
		
		for (i = 0, j = k; i < strlen(searchedWord) && j < strlen(aLine); ++i,++j)
			temp[i] = aLine[j];

		if(strcmp(temp,searchedWord) == 0) {
			++counter;
			if(ifWordExist == false){
				ifWordExist = true;
				++*lineNumber;
				fprintf(outputFile, "\n%s line: %d column: ",searchedWord,*lineNumber);
			}
			fprintf(outputFile,"%d, ",k + 1);			//column number
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
int searchInFile(const char fileName[40], const char *searchedWord) {
	
	FILE *pFileInput;
	FILE *pFileOutput;

	int i = 0,
		bytesToRead = 0, 
		totalFoundWord = 0,
		lineNumber = 0;
	char *aLine;

	pFileInput = fopen(fileName,"r");
	pFileOutput = fopen("gfF.log","a+");

	if ( pFileInput == NULL){
		printf("%s could not be opened!\n",*fileName );
		return ERROR_CODE;
	}

	if( pFileOutput == NULL) {
		printf("Output file can not be opened!\n");
		return ERROR_CODE;
	}

	bytesToRead = countLongestLine(pFileInput);

	rewind(pFileInput);

	aLine = (char *)malloc(sizeof(char) * bytesToRead);
	
	fprintf(pFileOutput, "%s",fileName );

	while(!feof(pFileInput)){
		readFromFile(pFileInput,bytesToRead,aLine);
		totalFoundWord += searchAWord(searchedWord,aLine,&lineNumber,pFileOutput);
	}

	fprintf(pFileOutput, "\n" );
	
	printf("Total word in the search: %d\n", totalFoundWord);

	free(aLine);


	fclose(pFileOutput);
	fclose(pFileInput);

	return 0;
}