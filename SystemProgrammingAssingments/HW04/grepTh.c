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

#include "grepTh.h"
#define DEBUG


int main(int argc, char const *argv[]) {
    char dirPaths[BUF_SIZE][BUF_SIZE];
    char topDirPath[BUF_SIZE];
	int i = 0, totalFoundWordNum = 0;
    pid_t childpid;
    int **pipeFD;
    struct ProcessData processData;

    if (argc != 3) {
        fprintf(stdout, "Usage: %s \"string\" <FullDirPath>\n",argv[0] );
        return 1;
    }

    memset(topDirPath,'\0',BUF_SIZE);
    unlink(LOG_FILE_NAME);

    if(getcwd(topDirPath, sizeof(topDirPath)) == NULL){
        perror("getcwd() error");
        return 0;
    }
    strcat(topDirPath,"/");
    strcat(topDirPath,argv[2]);

    #ifdef DEBUG
    fprintf(stderr, "DEBUG***topDirPath:%s\n",topDirPath );
    #endif

    if (ERROR == traversePath(topDirPath,dirPaths))
		return ERROR;

    snprintf(dirPaths[dirPathCounter],BUF_SIZE,"%s",topDirPath);
    ++dirPathCounter;

    // if (getnamed(SEM_NAME,&semLockp,1) == ERROR) {
    //     perror("Failed to create named semaphore");
    //     return ERROR;
    // }

    #ifdef DEBUG
    fprintf(stderr, "DEBUG***dirPathCounter: %d\n",dirPathCounter );
    fprintf(stderr, "DEBUG***topDirPath: %s\n",topDirPath );
    fprintf(stderr, "DEBUG**********************************\n" );
    for (i = 0; i < dirPathCounter; i++) {
        fprintf(stderr, "DEBUG***dirPath[%d]: %s\n",i,dirPaths[i] );
    }
    #endif

    pipeFD = (int **)calloc(dirPathCounter, sizeof(int *));

    for (i = 0; i < dirPathCounter; i++) {
        pipeFD[i] = (int *)calloc(2,sizeof(int));
    }

    for (i = 0; i < dirPathCounter; i++) {
        if (pipe(pipeFD[i]) == ERROR) {
            perror("Failed to create the pipe");
            exit(EXIT_FAILURE);
        }
    }
    if (clock_gettime(CLOCK_REALTIME,&firstTime)) {
        perror("Failed to get a starting time");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < dirPathCounter; ++i){
		if((childpid = fork()) <= 0)
			break;
	}

	if(childpid == 0){
        //close read of pipe
        // #ifdef DEBUG
        // fprintf(stderr, "DEBUG_CHILD***PID:%d\n",(int)getpid() );
        // #endif

        if (close(pipeFD[i][0]) == ERROR) {
            perror("Failed to close which is reading side of pipe");
            exit(EXIT_FAILURE);
        }

		startProcess(dirPaths[i],pipeFD[i][1],argv[1]); //argv[1]: searchedWord
		exit(EXIT_SUCCESS);
	} else if(childpid == -1){
		perror("Error in fork. Program will be terminated.");
		exit(EXIT_FAILURE);
	} else { //parent

        while( wait(NULL) != -1);

        long timedif;
        struct OutputData outputData;
        memset(&outputData,0,sizeof(outputData));
        outputData.totalCascadeThreadNum = 0;
        outputData.totalSearchedLineNum = 0;
        outputData.totalThreadNum = 0;
        outputData.totalSearcherThreadNum = 0;
        outputData.totalSearchedDirNum = 0;
        outputData.totalRunTime = 0;

        for (i = 0; i < dirPathCounter; i++){
            if (close(pipeFD[i][1]) == ERROR){
                perror("Failed to close which is writing side of pipe");
                exit(EXIT_FAILURE);
            }

            read(pipeFD[i][0],&processData,sizeof(processData));

            outputData.totalSearchedLineNum += processData.searchedLineNumber;
            outputData.totalThreadNum += processData.maxThreadNumber;
            outputData.totalSearchedFileNum += processData.searchedFileNumber;
            outputData.totalSearcherThreadNum += processData.searcherThNum;
            if (outputData.totalCascadeThreadNum < processData.liveThreadNum) {
                outputData.totalCascadeThreadNum = processData.liveThreadNum;
            }


        }
        if (clock_gettime(CLOCK_REALTIME,&secondTime)) {
            perror("Failed to finishing time");
            exit(EXIT_FAILURE);
        }
        timedif = BILLION * (secondTime.tv_sec - firstTime.tv_sec) +
                    (secondTime.tv_nsec - firstTime.tv_nsec);
        timedif = timedif / MILLION;
        outputData.totalRunTime = timedif;
        outputData.totalSearchedDirNum = dirPathCounter;

        for (i = 0; i < dirPathCounter; i++) {
            free(pipeFD[i]);
        }
        free(pipeFD);
        printToTerminal(outputData);
	}
}

/**
* Toplam bulunan kelime sayisini bulur ve bunu log.txt dosyasina basar
*
*/
void printToTerminal(struct OutputData outputData){
    FILE *pFileOutput;
    int counter = 0;
    char temp = '\0';
    pFileOutput = fopen(LOG_FILE_NAME,"a+");

    while (!feof(pFileOutput)){
        temp = fgetc(pFileOutput);
        if (temp == NEW_LINE) {
            ++counter;
        }
    }

    fclose(pFileOutput);
    fprintf(stdout,"Total number of strings found:         %d\n",counter);
    fprintf(stdout,"Number of directories searched:        %d\n",outputData.totalSearchedDirNum );
    fprintf(stdout,"Number of files searched:              %d\n",outputData.totalSearchedFileNum );
    fprintf(stdout,"Number of lines searched:              %d\n",outputData.totalSearchedLineNum);
    fprintf(stdout,"Number of cascade threads created:     %d\n",outputData.totalCascadeThreadNum);
    fprintf(stdout,"Number of search threads created:      %d\n",outputData.totalSearcherThreadNum);
    fprintf(stdout,"Max %d of threads running concurrently.  \n",outputData.totalThreadNum);
    fprintf(stdout,"Total run time, %d in milliseconds.      \n",outputData.totalRunTime );

}


/*
 * Reference: https://github.com/bias/usp_exercises/blob/master/ch05/traversing-directories/mydu.c
 */
int traversePath(const char *dirPath, char dirPaths[][BUF_SIZE]){
	char buf[BUF_SIZE];
	struct stat statbuf;
	struct dirent *direntp;
	DIR *pDir;

	if ((pDir = opendir(dirPath)) == NULL){
		fprintf(stderr, "%s path can not open! Program will be terminated: %s\n",
				dirPath,strerror(errno) );
		return ERROR;
	}

	chdir(dirPath);
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
                snprintf(dirPaths[dirPathCounter],BUF_SIZE,"%s",buf);
                ++dirPathCounter;
				traversePath(buf,dirPaths);
			}
        }
	}
	chdir("../");
	while ((closedir(pDir) == -1) && (errno == EINTR));
    free(direntp);
	return 0;
}

void startProcess(char dirPath[BUF_SIZE], int wPipeFD, const char *searchedWord){
    char buf[BUF_SIZE],
         filePaths[BUF_SIZE][BUF_SIZE];
    int filePathsCounter = 0, // thread number (cascade)
        rowNumber = 0,
        i = 0,
        errorTh,
        error;
    struct ThreadData *threadData;
    struct ThreadReturnData *thReturnData;
    struct ProcessData processData;
    struct stat statbuf;
    struct dirent *direntp;
    DIR *pDir;
    pthread_attr_t threadAttr;

    #ifdef DEBUG
    fprintf(stderr, ANSI_COLOR_RED
                "\n-----------------START_PROCESS_1-------------------------"
                                                            ANSI_COLOR_RESET"\n");
    fprintf(stderr, ANSI_COLOR_RED"\nDEBUG_CHILD:%d:*****dirPath: %s"ANSI_COLOR_RESET"\n",
                                                    (int)getpid(),dirPath );
    fprintf(stderr, ANSI_COLOR_RED"DEBUG_CHILD:%d:*****searchedWord: %s"ANSI_COLOR_RESET"\n",
                                                    (int)getpid(),searchedWord );
    #endif

    if ((pDir = opendir(dirPath)) == NULL) {
        fprintf(stderr, "%s path can not open! Program will be terminated** : %s\n",
                                    dirPath,strerror(errno));
        exit(EXIT_FAILURE);
    }

    // file paths are collected.
    while((direntp = readdir(pDir)) != NULL){
        if ( strcmp(direntp->d_name, ".") != 0 &&
            strcmp(direntp->d_name, "..") != 0 ) {
            strcat(strcat(strcpy(buf, dirPath), "/"), direntp->d_name);

            if (lstat(buf, &statbuf) == ERROR) {
                perror("Couldn't get file status");
                fprintf(stderr, "\t%s/%s\n",dirPath,direntp->d_name );
                exit(EXIT_FAILURE);
            }
            if (S_ISREG(statbuf.st_mode)) {
                strcpy(filePaths[filePathsCounter],buf);
                ++filePathsCounter;
            }
        }
    }
    while ((closedir(pDir) == -1) && (errno == EINTR));

    #ifdef DEBUG
    fprintf(stderr, ANSI_COLOR_RED "DEBUG_CHILD:%d:****filePathsCounter: %d"ANSI_COLOR_RESET"\n",
                                                (int)getpid(),filePathsCounter );
    for (i = 0; i < filePathsCounter; i++) {
        fprintf(stderr, ANSI_COLOR_RED "DEBUG_CHILD:%d:****filePaths[%d]: %s"ANSI_COLOR_RESET"\n",
                                                    (int)getpid(),i,filePaths[i] );
    }
    #endif

    threadData = (struct ThreadData *) calloc (filePathsCounter,sizeof(struct ThreadData));
    thReturnData = (struct ThreadReturnData *) calloc(1,sizeof(struct ThreadReturnData));
    thReturnDataG = (struct ThreadReturnData *) calloc(1,sizeof(struct ThreadReturnData));
    processData.searchedLineNumber = 0;
    processData.searchedFileNumber = 0;

    for (i = 0; i < filePathsCounter; i++) {
        strcpy(threadData[i].filePath,filePaths[i]);
        strcpy(threadData[i].searchedWord,searchedWord);
        threadData[i].processID = getpid();
    }

    for (i = 0; i < filePathsCounter; ++i) {
        pthread_attr_init(&threadAttr);
        pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);
        if (errorTh = pthread_create(&threadData[i].thID,NULL,startThread,&threadData[i])) {
            fprintf(stderr, "Failed to create thread %d: %s\n",i,strerror(errorTh));
            threadData[i].thID = pthread_self();
        }
        sleep(1);
    }

    for (i = 0; i < filePathsCounter; i++){
        if (pthread_equal(pthread_self(),threadData[i].thID)) {
            continue;
        }

        if (error = pthread_join(threadData[i].thID,&thReturnData)) {
            fprintf(stderr, "Failed to join thread %d: %s\n",i,strerror(error));

        }
        #ifdef DEBUG
        fprintf(stderr, ANSI_COLOR_RED
            "DEBUG:%d:******thReturnData[%d].searchedLineNumber:%d"
                ANSI_COLOR_RESET"\n",
                                (int)getpid(),i,thReturnData->searchedLineNumber);
        #endif

        processData.searchedLineNumber += thReturnData->searchedLineNumber;
    }
    processData.searchedFileNumber = filePathsCounter;
    processData.maxThreadNumber = filePathsCounter;
    processData.liveThreadNum = liveThreadNum;
    processData.searcherThNum = searcherThNum;

    #ifdef DEBUG
    fprintf(stderr,ANSI_COLOR_RED "DEBUG_CHILD:%d:*****liveThreadNum: %d"ANSI_COLOR_RESET"\n"
                                                ,(int)getpid(),liveThreadNum );
    fprintf(stderr,ANSI_COLOR_RED "DEBUG_CHILD:%d:*****searcherThNum: %d"ANSI_COLOR_RESET"\n"
                                                ,(int)getpid(),searcherThNum );
    fprintf(stderr,ANSI_COLOR_RED
            "DEBUG_CHILD:%d:*****processData.maxThreadNumber: %d"ANSI_COLOR_RESET"\n",
                        (int)getpid(),processData.maxThreadNumber );
    fprintf(stderr,ANSI_COLOR_RED
        "DEBUG_CHILD:%d:*****processData.searchedLineNumber: %d"ANSI_COLOR_RESET"\n",
                                (int)getpid(),processData.searchedLineNumber );
    fprintf(stderr,ANSI_COLOR_RED
            "-----------------START_PROCESS_3-------------------------\n\n"
                                                                ANSI_COLOR_RESET);
    #endif

    write(wPipeFD,&processData,sizeof(processData));
    free(thReturnData);
    free(threadData);
    free(direntp);
    exit(EXIT_SUCCESS);

}

/**
 * This function is drive function. All operations are applied in this function.
 *
 * @param	filePath		this file path is founded
 * @param 	*searchedWord	this word is wanted word in the file
 */
void *startThread(void *datas){
    FILE *pFileInput;
    int rowNumber = 0, // searched line number
        i = 0,
        **rowLineTable,
        allCharNum = 0,
        longestLineNum = 0;
    char *fileContent;
    struct ThreadData *threadData = (struct ThreadData *)datas;


    //sleep(1);

    pthread_mutex_lock(&muLock);
    ++liveThreadNum;
    pthread_mutex_unlock(&muLock);

    #ifdef DEBUG
    fprintf(stderr, "\n---------------------START_THREAD_1--------------------\n");
    fprintf(stderr, "DEBUG_THREAD:%d:*****threadData->processID: %d\n",
                                (int)syscall(SYS_gettid),threadData->processID );
    fprintf(stderr, "DEBUG_THREAD:%d:*****threadData->thID: %d\n",
                                (int)syscall(SYS_gettid),(int)threadData->thID );
    fprintf(stderr, "DEBUG_THREAD:%d:*****threadData->filePath: %s\n",
                                    (int)syscall(SYS_gettid),threadData->filePath );
    fprintf(stderr, "DEBUG_THREAD:%d:*****threadData->searchedWord: %s\n",
                            (int)syscall(SYS_gettid),threadData->searchedWord );
    fprintf(stderr, "DEBUG_THREAD:%d*****threadData->pipeFD: %d\n",
                                (int)syscall(SYS_gettid),threadData->processID );
    #endif

    pFileInput = fopen(threadData->filePath,"r");

    if (pFileInput == NULL) {
        fprintf(stderr, "%s could not be opened!\n",threadData->filePath );
        pthread_mutex_lock(&muLock);
        thReturnDataG->errorNo = errno;
        thReturnDataG->searchedLineNumber = 0;
        --liveThreadNum;
        pthread_mutex_unlock(&muLock);
        return (void * )thReturnDataG;
    }

    rowNumber = countRowNumber(pFileInput);
    #ifdef DEBUG
    fprintf(stderr, "DEBUG_THREAD:%d:*****rowNumber: %d\n",(int)syscall(SYS_gettid),rowNumber );
    fprintf(stderr, "---------------------START_THREAD_1--------------------\n\n");
    #endif

    rowLineTable = (int **) calloc(sizeof(int *) , (rowNumber+1));


    if (rowLineTable == NULL) {
        fprintf(stderr, "rowLineTable is NULL\n" );
        pthread_mutex_lock(&muLock);
        thReturnDataG->errorNo = ERROR;
        thReturnDataG->searchedLineNumber = 0;
        --liveThreadNum;
        pthread_mutex_unlock(&muLock);
        return (void * )thReturnDataG;
    }

    for (i = 0; i < rowNumber; i++) {
        rowLineTable[i] = (int *) calloc(sizeof(int) , 2);
        if (rowLineTable[i] == NULL) {
            pthread_mutex_lock(&muLock);
            thReturnDataG->errorNo = ERROR;
            thReturnDataG->searchedLineNumber = 0;
            --liveThreadNum;
            pthread_mutex_unlock(&muLock);
            return (void * )thReturnDataG;
        }
    }

    rewind(pFileInput);

    longestLineNum = fillToRowLineTable(pFileInput,rowNumber,rowLineTable);
    rewind(pFileInput);

    allCharNum = rowLineTable[rowNumber-1][1];

    fileContent = (char *) calloc(sizeof(char) , (allCharNum+1));
    memset(fileContent, 0, allCharNum+1);

    readFromFile(pFileInput,fileContent,longestLineNum);

    searchInFileContent(fileContent,rowLineTable,rowNumber,threadData,threadData->filePath);

    free(fileContent);
    for (i = 0; i < rowNumber; i++) {
        free(rowLineTable[i]);
    }
    pthread_mutex_lock(&muLock);
    thReturnDataG->errorNo = 0;
    thReturnDataG->searchedLineNumber = rowNumber;
    pthread_mutex_unlock(&muLock);

    free(rowLineTable);
    fclose(pFileInput);
    return thReturnDataG;
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
                         const struct ThreadData *thData,
                            char filePath[BUF_SIZE]){
    FILE *pFileOutput;
    int i = 0,j = 0, k = 0, m = 0;
    char *temp, *fileName, *fileNameTemp;
    int allChar = rowLineTable[rowNumber-1][1];

    pthread_mutex_lock(&muLock);
    ++searcherThNum;
    pthread_mutex_unlock(&muLock);

    temp = (char *)calloc((strlen(thData->searchedWord) + 1) , sizeof(char));

    pFileOutput = fopen(LOG_FILE_NAME,"a+");

    fileNameTemp = strtok(filePath,"/");
    while (fileNameTemp != NULL) {
        fileNameTemp = strtok(NULL,"/");
        if (fileNameTemp != NULL) {
            fileName = fileNameTemp;
        }
    }

    // #ifdef DEBUG
    // fprintf(stderr, "-------------------SEARCHINFILECONTENT-----------------\n");
    // fprintf(stderr, "DEBUG_THREAD:%d:*****fileName: %s\n",(int)syscall(SYS_gettid),fileName );
    // #endif

    for (i = 0; i < allChar; i++) {
        for (j = i, k = 0; k < strlen(thData->searchedWord) &&  j < allChar; j++) {
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
        if (strcmp(temp,thData->searchedWord) == 0) {
            for(m = 0; m < strlen(thData->searchedWord); ++m){
                temp[m] = '\0';
            }
            //pthread_mutex_lock(&muLock);
            printFoundWordLocation(rowLineTable,i+1,rowNumber,thData,pFileOutput,fileName);
            //pthread_mutex_unlock(&muLock);
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
                            int rowNumber,const struct ThreadData *thData,
                            FILE *pFileOutput,char *fileName) {
    int i = 0;

    if (pFileOutput == NULL) {
        perror("Log file could not be opened!" );
        // pthread_exit(0);
    }
    for (i = 0; i < rowNumber; i++) {
        if(foundedColumn < rowLineTable[i][1]){
            if(i == 0){
                fprintf(pFileOutput, "%d - %ld %s :[%d,%d] %s first character is found\n",
                        thData->processID, (long)syscall(SYS_gettid),
                        fileName,rowLineTable[i][0],foundedColumn,thData->searchedWord);
            }else{
                fprintf(pFileOutput, "%d - %ld %s :[%d,%d] %s first character is found\n",
                        thData->processID,(long)syscall(SYS_gettid),
                        fileName,rowLineTable[i][0],
                            (foundedColumn - rowLineTable[i-1][1]),thData->searchedWord);
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
