/*
 * HW03_121044029_Mehmet_Gurol_Cay
 *
 * File: listdir.c
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
#include "newListDir.h"

//#define  DEBUG_DIR_CHILD
//#define DEBUG
//#define FIFO_DEBUG

int main(int argc, char const *argv[]) {
    char dirPath[BUF_SIZE];
	int i = 0, totalFoundWordNum = 0, fifoFD;
    pid_t pid;

    if (argc != 3) {
        fprintf(stdout, "Usage: %s \"string\" <DirName>\n",argv[0] );
        return 1;
    }

    memset(dirPath,'\0',BUF_SIZE);

    if(getcwd(dirPath, sizeof(dirPath)) == NULL){
        perror("getcwd() error");
        return 0;
    }
    strcat(dirPath,"/");
    strcat(dirPath,argv[2]);

    

    searchedWord = (char *)calloc(strlen(argv[1]),sizeof(char));
    strcat(searchedWord,argv[1]);


    wrapper(dirPath);
    free(searchedWord);
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
int getDirAndFileNum(const char *dirPath,int *dirNum, int *fileNum){
	char buf[BUF_SIZE];
	struct stat statbuf;
	struct dirent *direntp;
	DIR *pDir;

	if ((pDir = opendir(dirPath)) == NULL){
		fprintf(stderr, "%s path can not open! Program will be terminated\n",
				dirPath );
		return ERROR;
	}
	while ((direntp = readdir(pDir)) != NULL) {
		if ( strcmp(direntp->d_name, ".") != 0 &&
			strcmp(direntp->d_name, "..") != 0 ) {
			strcat(strcat(strcpy(buf, dirPath), "/"), direntp->d_name);

			if ( lstat(buf, &statbuf) == -1) {
				perror("Couldn't get file status");
				fprintf(stderr, "\t%s/%s\n", dirPath, direntp->d_name);
				return ERROR;
			}
			if ( S_ISDIR(statbuf.st_mode) ) { // dir
                ++(*dirNum);
			}
			else if ( S_ISREG(statbuf.st_mode) ) { // files
				++(*fileNum);
			}
        }
	}
	while ((closedir(pDir) == -1) && (errno == EINTR));

	return 0;
}

void wrapper(const char *dirPath)
{
	fprintf(stderr, "Searching word: %s\n",searchedWord );	

	
	int childpid = fork();

	if(childpid==0)
	{
		startSystem(dirPath);
		exit(1);
	}
	else if(childpid <0)
	{
		perror("error in fork");
		return;
	}
	else
	{
		char fifoName[30]="";
		sprintf(fifoName,"fifo%d",childpid);

		if ((mkfifo(fifoName, FIFO_PERMS) == -1) && (errno != EEXIST))
		{
			perror("Failed to create the fifo\n");
			exit(-1);
		}
		#ifdef FIFO_DEBUG
		fprintf(stderr, "Created fifo:%s\n",fifoName);
		#endif

    	#ifdef FIFO_DEBUG
		fprintf(stderr, "DEBUG: Opening fifo that name is %s in read mode...\n",fifoName);
		#endif

		int readfifofd;
		if ((readfifofd = open(fifoName, O_RDONLY)) == -1)
		{
			fprintf(stderr, "%s:%s\n",strerror(errno),fifoName );
		}
		#ifdef FIFO_DEBUG
		fprintf(stderr, "DEBUG: %s is opened as read mode.\n",fifoName);
		#endif


		#ifdef FIFO_DEBUG
		fprintf(stderr, "DEBUG: Reading from %s.\n",fifoName);
		#endif

		char readString[PIPE_BUF];
		if(read(readfifofd, readString, PIPE_BUF)==-1)
		{
			perror("Error reading in fifo");
		}

		#ifdef FIFO_DEBUG
		fprintf(stderr, "This word is read from mainFifo:%s",readString );
		#endif

		if( unlink(fifoName) == -1 )
		{
			perror("Failed to remove fifo\n");
		}

		fprintf(stderr, "MAIN CODE result\n|%s|\n",readString);
	}
}

void startSystem(const char *dirPath){
    DIR *pDir;
    struct stat statbuf;
    struct dirent *direntp;
    char buf[BUF_SIZE],readPipe[BUF_SIZE],tempBuf[BUF_SIZE];
    int dirNum = 0, fileNum = 0,i = 0, k = 0, m = 0, readFifoFD,writeFifoFD,tempFD;
    pid_t childPID;
    char fifoName[MAX_FILE_NAME],temp[MAX_FILE_NAME];
    char logRecord[1024];
    fileInf_t *files;

    getDirAndFileNum(dirPath,&dirNum,&fileNum);
    memset(fifoName,'\0',MAX_FILE_NAME);

    files = (fileInf_t *)calloc((fileNum + dirNum), sizeof(fileInf_t));

    for (i = 0; i <(dirNum+fileNum); ++i){
        files[i].filePath = (char *)calloc(MAX_FILE_NAME,sizeof(char));
    }

    if ((pDir = opendir(dirPath)) == NULL){
		fprintf(stderr, "%s path can not open! Program will be terminated\n",
				dirPath );
		exit(ERROR);
	}

    snprintf(fifoName,MAX_FILE_NAME,"%ld.fifo",(long)getpid());

	while ((direntp = readdir(pDir)) != NULL && (k < (fileNum + dirNum))) {
		if ( strcmp(direntp->d_name, ".") != 0 &&
			strcmp(direntp->d_name, "..") != 0 ) {
			strcat(strcat(strcpy(buf, dirPath), "/"), direntp->d_name);

			if ( lstat(buf, &statbuf) == ERROR) {
				perror("Couldn't get file status");
				fprintf(stderr, "\t%s/%s\n", dirPath, direntp->d_name);
				exit(ERROR);
			}
			if ( S_ISDIR(statbuf.st_mode) ) { // dir
                strcpy(files[k].filePath,buf);
                files[k].fileStatus = DIR_STTS;
			}
			else if ( S_ISREG(statbuf.st_mode) ) { // files
                strcpy(files[k].filePath,buf);
                files[k].fileStatus = FILE_STTS;
			}
            ++k;
        }
	}

	while ((closedir(pDir) == -1) && (errno == EINTR));

    /*#ifdef DEBUG
    for (i = 0; i < fileNum + dirNum; i++) {
        fprintf(stderr, "filePath: %s --> status: %d\n",files[i].filePath,files[i].fileStatus);
    }
    #endif*/

    for ( i = 0; i < (dirNum + fileNum); i++) {
        if( pipe(files[i].pipeFD) == ERROR){
            perror("Failed to create the pipe");
            exit(EXIT_FAILURE);
        }
    }


    int childPIDArr[dirNum];
    for ( i = 0; i < (dirNum); i++) 
		childPIDArr[i]=0;
    int index=0;

    for (i = 0; i < (dirNum+fileNum) ; ++i){
        if ((childPID = fork()) <= 0)
            break;
        else
        {
        	if(DIR_STTS == files[i].fileStatus)
        	{
        		childPIDArr[index++]=childPID;

        		char fifoName[30]="";
				sprintf(fifoName,"fifo%d",childPID);

				if ((mkfifo(fifoName, FIFO_PERMS) == -1) && (errno != EEXIST))
				{
					perror("Failed to create the fifo\n");
					exit(-1);
				}
				#ifdef FIFO_DEBUG
				fprintf(stderr, "DEBUG: Created fifo:%s.\n",fifoName);
				#endif
        	}
        }
    }

    if (childPID == 0) {
        if (FILE_STTS == files[i].fileStatus){
            if (close(files[i].pipeFD[0]) == ERROR){
                perror("Failed to close which is reading side of pipe");
                exit(ERROR);
            }
            startSearch(files[i].filePath,files[i].pipeFD[1]);
            exit(SUCCESS);
        } else if (DIR_STTS == files[i].fileStatus ) {

        	#ifdef DEBUG_DIR_CHILD
        	fprintf(stderr, "I am child and my id is:%d\n",getpid());
        	#endif

            startSystem(files[i].filePath);            
            
        	exit(SUCCESS);
        }
    } else if (childPID == ERROR) {
        perror("Failed fork");
        exit(ERROR);
    } else {
    	// parent code

    	#ifdef DEBUG
		fprintf(stderr, "my pid:%d, my dir childsize:%d, mychild first pid:%d\n",getpid(),index,childPIDArr[0]);
    	#endif

    	// READ FROM FIFO
    	int forIndex = 0;
    	for(;forIndex<index;++forIndex)
    	{
	    	char fifoName[30]="";
			sprintf(fifoName,"fifo%d",childPIDArr[forIndex]);
	    	#ifdef FIFO_DEBUG
			fprintf(stderr, "DEBUG: Opening %s in read mode...\n",fifoName);
			#endif

			int readfifofd;
			if ((readfifofd = open(fifoName, O_RDONLY)) == -1)
			{
				fprintf(stderr, "%s:%s\n",strerror(errno),fifoName );
			}
			#ifdef FIFO_DEBUG
			fprintf(stderr, "DEBUG: %s is opened as read mode.\n",fifoName);
			#endif


			#ifdef FIFO_DEBUG
			fprintf(stderr, "DEBUG: Reading from %s.\n",fifoName);
			#endif

			char readString[PIPE_BUF];
			if(read(readfifofd, readString, PIPE_BUF)==-1)
			{
				perror("Error reading in fifo");
				return;
			}

			#ifdef FIFO_DEBUG
			fprintf(stderr, "This word is read from mainFifo:%s",readString );
			#endif

			if( unlink(fifoName) == -1 )
			{
				perror("Failed to remove the fifo\n");
				return;
			}
			else
				fprintf(stderr, "Removed %s.\n",fifoName);


			strcat(logRecord,readString);
		}


		// READ FROM PIPE
        while(wait(NULL) != ERROR);

        for (k = 0; k < fileNum + dirNum; k++) {
            if (close(files[k].pipeFD[1]) == ERROR){
                //fprintf(stderr, "************pipeFD[%d][1]: %d\n",k,files[k].pipeFD[1] );
                perror("Failed to close write side of pipe file descriptor");
            }

            do{
                if (read(files[k].pipeFD[0],readPipe,BUF_SIZE) == ERROR){
                    perror("Failed to read from pipe");
                    exit(ERROR);
                }
                if(strcmp(readPipe,"done!\n"))
                	strcat(logRecord,readPipe);

            }while(strcmp(readPipe,"done!\n") != 0);
        }
        close(writeFifoFD);
        for(k = 0; k < fileNum + dirNum; k++){
            free(files[k].filePath);
        }
        free(files);



        // WRITE TO FIFO

        char myFifoName[30]="";
		sprintf(myFifoName,"fifo%d",getpid());
        #ifdef FIFO_DEBUG
		fprintf(stderr, "DEBUG: Opening %s as write mode.\n",myFifoName);
		#endif
		int writefifofd;
		if ((writefifofd = open(myFifoName, O_WRONLY)) == -1)
		{
			fprintf(stderr,"Failed to open the %s, Error::%s \n",myFifoName,strerror(errno));
			return;
		}
		#ifdef FIFO_DEBUG
		fprintf(stderr, "DEBUG: Opened %s as write mode.\n",myFifoName);
		#endif


		#ifdef FIFO_DEBUG
		fprintf(stderr, "DEBUG: Writing %s to mainFifo.\n",logRecord);
		#endif
		int length = strlen(logRecord);
		if(r_write(writefifofd, logRecord, length) != length)
		{
			perror("Failed to write fifo\n");
			return;
		}
		#ifdef FIFO_DEBUG
		fprintf(stderr, "This word is written to %s:%s",myFifoName,logRecord);
		#endif

    }
}

/**
 * This function is drive function. All operations are applied in this function.
 *
 * @param	filePath		this file path is founded
 * @param 	*searchedWord	this word is wanted word in the file
 */
void startSearch(char filePath[BUF_SIZE],int wPipeFD){
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

    searchInFileContent(fileContent,rowLineTable,rowNumber,filePath,wPipeFD);

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
                            char filePath[BUF_SIZE],int wPipeFD){
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
            printFoundWordLocation(rowLineTable,i+1,rowNumber,pFileOutput,fileName,wPipeFD);
        }
    }

    if(write(wPipeFD,"done!\n",BUF_SIZE) == ERROR){
        perror("!!!!!!Failed to write pipe");
        exit(ERROR);
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
                            int rowNumber,FILE *pFileOutput,char *fileName,
                            int wPipeFD) {
    int i = 0;
    char buffer[BUF_SIZE];

    memset(buffer,'\0',BUF_SIZE);

    if (pFileOutput == NULL) {
        perror("Log file could not be opened!" );
        exit(0);
    }
    for (i = 0; i < rowNumber; i++) {
        if(foundedColumn < rowLineTable[i][1]){
            if(i == 0){
                snprintf(buffer,BUF_SIZE,"%s:[%d,%d] first character is found\n",
                        fileName,rowLineTable[i][0],foundedColumn);

                if(write(wPipeFD,buffer,BUF_SIZE) == ERROR){
                    perror("(((Failed to write pipe");
                    exit(ERROR);
                }
                // fprintf(stderr, "%s:[%d,%d] first character is found\n",
                //         fileName,rowLineTable[i][0],foundedColumn);

            }else{
                snprintf(buffer,BUF_SIZE,"%s:[%d,%d] first character is found\n",
                        fileName,rowLineTable[i][0],(foundedColumn - rowLineTable[i-1][1]));

                if(write(wPipeFD,buffer,BUF_SIZE) == ERROR){
                    perror(")))Failed to write pipe");
                    exit(ERROR);
                }
                // fprintf(stderr, "%s:[%d,%d] first character is found\n",
                //         fileName,rowLineTable[i][0],(foundedColumn - rowLineTable[i-1][1]));

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


ssize_t r_write(int fd, void *buf, size_t size)
{
	char *bufp;
	size_t bytestowrite;
	ssize_t byteswritten;
	size_t totalbytes;

	for (bufp = buf, bytestowrite = size, totalbytes = 0;
		bytestowrite > 0;
		bufp += byteswritten, bytestowrite -= byteswritten)
	{
		byteswritten = write(fd, bufp, bytestowrite);
		if ((byteswritten) == -1 && (errno != EINTR))
			return -1;
		if (byteswritten == -1)
			byteswritten = 0;
		totalbytes += byteswritten;
	}
	return totalbytes;
}

