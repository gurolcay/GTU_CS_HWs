/*
 * HW01_121044029_Mehmet_Gurol_Cay
 *
 * File: main.c
 * Author: Mehmet Gürol Çay
 *
 * Created on March 7, 2017, 07:45 PM
 *
 * Description
 * -----------
 * This program searches a word in the given file. This program give to output
 * on screen. Ignored character are 'tab', '\n' and white space.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_CODE -1
#define NEW_LINE '\n'     // NEW LINE ASCII CODE
#define WHITE_SPACE ' '  // WHITE SPACE ASCII CODE
#define TAB '\t'

void startSearch(const char *fileName,const char *searchedWord);
void readFromFile(FILE *pFileInput,char *fileContent,int longestLineNum);
void searchInFileContent(const char *fileContent,  int **rowLineTable,int rowNumber,
                         const char *searchedWord);
void printFoundWordLocation(int **rowLineTable, int foundedColumn,
                            int rowNumber,const char *searchedWord);
int fillToRowLineTable(FILE *pFileInput,int rowNumber,int **rowLineTable);
int countRowNumber(FILE *pFileInput);
int memoryAllocForTable(int **rowLineTable,int rowNumber);

int main(int argc, char const *argv[]) {

    if (argc != 3) {
        fprintf(stdout, "Usage: %s \"string\" <filename.txt>\n",argv[0] );
        return 1;
    }

    startSearch(argv[2],argv[1]); //argv[2]: fileName argv[1]: searchedWord

}

/**
* The startSearch function is used to search for the desired sheen in the file
* Call the helper functions to make the searching process
*
* @param    *fileName          file name to check content
* @param    *searchedWord      search word in file
*/
void startSearch(const char *fileName,const char *searchedWord){
    FILE *pFileInput;

    int rowNumber,i = 0, **rowLineTable, allCharNum = 0, longestLineNum = 0;
    char *fileContent;


    pFileInput = fopen(fileName,"r");

    if (pFileInput == NULL) {
        perror("Input file couldn't opened: ");
        exit(1);
    }

    rowNumber = countRowNumber(pFileInput);

    rowLineTable = (int **) malloc(sizeof(int *) * (rowNumber+1));

    if (rowLineTable == NULL) {
        exit(1);
    }

    for (i = 0; i < rowNumber; i++) {
        rowLineTable[i] = (int *) malloc(sizeof(int) * 2);
        if (rowLineTable[i] == NULL) {
            exit(1);
        }
    }

    rewind(pFileInput);

    longestLineNum = fillToRowLineTable(pFileInput,rowNumber,rowLineTable);

    rewind(pFileInput);


    allCharNum = rowLineTable[rowNumber-1][1];


    fileContent = (char *) malloc(sizeof(char) * (allCharNum+1));

    readFromFile(pFileInput,fileContent,longestLineNum);

    searchInFileContent(fileContent,rowLineTable,rowNumber,searchedWord);

    free(fileContent);
    for (i = 0; i < rowNumber; i++) {
        free(rowLineTable[i]);
    }
    free(rowLineTable);
    fclose(pFileInput);
}
/**
* It performs the search for the phrase in fileContent. When the searchedWord is
* found, it is printed on the screen which line and column of the sheath is found
*
* @param    fileContent     the file contents are stored in char pointer
* @param    rowLineTable    table that holds the row number and the number of
*                           charactes in the previous row
* @param    rowNumber       number of lines in file
* @param    searchedWord    the word to search for in the file
*/
void searchInFileContent(const char *fileContent, int **rowLineTable, int rowNumber,
                         const char *searchedWord){
    int i = 0,j = 0, k = 0, m = 0;
    char *temp;
    int allChar = rowLineTable[rowNumber-1][1];

    temp = (char *)malloc((strlen(searchedWord) + 1) * sizeof(char));

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
        if (strcmp(temp,searchedWord) == 0)
        {
            for(m = 0; m < strlen(searchedWord); ++m){
                temp[m] = '\0';
            }
            printFoundWordLocation(rowLineTable,i+1,rowNumber,searchedWord);
        }
    }

    free(temp);
}
/**
* Calculates the line and the milk in which the found is located and prints it
* on the screen
*
* @param    rowLineTable    table that holds the row number and the number of
*                           charactes in the previous row
* @param    foundedColumn      the starting column in the file of the searched word
* @param    rowNumber          line number of the file
* @param    searchedWord       the word to search for in the file
*/
void printFoundWordLocation(int **rowLineTable, int foundedColumn,
                            int rowNumber,const char *searchedWord) {
    int i = 0;
    for (i = 0; i < rowNumber; i++) {
        if(foundedColumn < rowLineTable[i][1]){
            //fprintf(stdout, "??????rowLineTable[%d][1]:%d\n",i,rowLineTable[i][1] );
            if(i == 0){
                fprintf(stderr, "%s row: %d column: %d\n",
                        searchedWord,rowLineTable[i][0],foundedColumn);
            }else{
                fprintf(stderr, "%s row: %d column: %d\n",
                        searchedWord,rowLineTable[i][0],(foundedColumn - rowLineTable[i-1][1]));
            }
            break;
        }
    }
}
/**
* The input file is read and fill the content to fileContent
*
* @param    pFileInput      input file pointer
* @param    fileContent     the file contents are stored in char pointer
* @param    longestLineNum  the longest line in the file
*/
void readFromFile(FILE *pFileInput,char *fileContent,int longestLineNum) {
    char *line;

    line = (char *)malloc(sizeof(char) * (longestLineNum+1));

    while(!feof(pFileInput)){
        if(fgets(line,longestLineNum,pFileInput) == NULL)
            break;
        strcat(fileContent,line);
    }
    free(line);
}
/**
* fills the number of characters in the line and previous lines which is in
* input file to the rowLineTable
*
* @param    pFileInput      input file pointer
* @param    rowNumber       line number of the file
* @param    rowLineTable    table that holds the row number and the number of
*                           charactes in the previous row
* @return                   the longest line in the file
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
* Find out how many lines the input file
*
* @param    pFileInput      input file pointer
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
