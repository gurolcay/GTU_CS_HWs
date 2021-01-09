/*
 * Midterm_121044029_Mehmet_Gurol_Cay
 *
 * timerServer.c
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

//#define DEBUG
#include "timerServer.h"

int main(int argc, char const *argv[]) {
	char dirPath[256];

    if (argc != 4){
        fprintf(stderr, "Usage: %s <ticts in miliseconds> <n> <mainFifoName>\n",argv[0] );
        return 1;
    }

    timeMiliSec = atoi(argv[1]);

    serverInf.matrixSize = 2 * atoi(argv[2]);
    if ( !(0 < serverInf.matrixSize  && serverInf.matrixSize <= MM_SIZE)) {
        fprintf(stderr, "Invalid matrix size. Please enter the size between 0 and %d\n",MM_SIZE/2);
        return ERROR;
    }
	memset(dirPath,'\0',sizeof(char));
	if (stat(strcat(getcwd(dirPath,sizeof(dirPath)),"/log/"), &st) == -1) {
	    mkdir(strcat(getcwd(dirPath,sizeof(dirPath)),"/log/"), 0700);
	}

	serverLog.matrixSize = serverInf.matrixSize;
    mainFifoName = (char *) calloc(strlen(argv[3]), sizeof(char) );
    strcat(mainFifoName,argv[3]);

    #ifdef DEBUG
    fprintf(stderr, "DEBUG****mainFifoName: %s\n",mainFifoName );
    #endif

    if ( (mkfifo(mainFifoName,FIFO_PERMS) == -1) && (errno != EEXIST) ) {
        fprintf(stderr, "Server failed to create the %s:%s\n",mainFifoName,strerror(errno) );
        return ERROR;
    }

	if (clock_gettime(CLOCK_REALTIME,&startServerTime)){
		perror("Failed to get first time");
		return ERROR;
	}

    readFromClient();

    return 0;
}
bool checkServerInput(const char *ms, const char *n){

}

void readFromClient(){
    int serverPID,
        clientPID,
        childPID;
    int signalNum;
	time_t curtime;

    serverInf.serverPID = (int)getpid();

    if (signal(SIGINT, SIGINT_handler) == SIG_ERR){
        perror("SIGINT install error");
		exit(EXIT_FAILURE);
    }

    if(signal(SIGUSR1,SIGUSR1_handler) == SIG_ERR){
        perror("SIGUSR1 install error");
		exit(EXIT_FAILURE);
    }

	if ((pServerLog = fopen(SERVER_LOG_NAME,"w+")) == NULL) {
		perror("Server log file was not opened");
		exit(EXIT_FAILURE);
	}

    while (1) {

        if ( (mainFifoFD = open(mainFifoName,O_RDWR) ) == ERROR) {
            #ifdef DEBUG
            fprintf(stderr, "mainFifoFD: %d\n",mainFifoFD );
            #endif

            fprintf(stderr, "****Server failed to open main fifo %s: %s\n",mainFifoName, strerror(errno));
            exit(EXIT_FAILURE);
        }

        memset(clientInf.clientFifoName,'\0',sizeof(char));

        //client fifo name was taken.
        if (r_read(mainFifoFD,&clientInf,sizeof(clientInf)) == ERROR){
            perror("Failed to read from main fifo");
            exit(EXIT_FAILURE);
        }
        close(mainFifoFD);

        #ifdef DEBUG
        fprintf(stderr, "DEBUG****Readed from mainfifo: %s\n",clientFifoName);
        #endif

        if ((clientFifoFD = open(clientInf.clientFifoName,O_WRONLY)) == ERROR) {
            perror("Client fifo could not be opened 1");
            exit(EXIT_FAILURE);
        }
		serverInf.timeMSec = timeMiliSec;
        //send serverPID
        if( write(clientFifoFD,&serverInf,sizeof(serverInf)) == ERROR){
            fprintf(stderr, "Failed to write server information %s\n",strerror(errno) );
            exit(EXIT_FAILURE);
        }
        #ifdef DEBUG
        fprintf(stderr, "DEBUG****clientFlag: %d\n",clientFlag );
        #endif

        pause();
        #ifdef DEBUG
        fprintf(stderr, "DEBUG****pass the pause\n");
        #endif

		if (clock_gettime(CLOCK_REALTIME,&clientConnectionTime)){
			perror("Failed to get a last connected client time");
			exit(1);
		}

		serverLog.clientPID = clientInf.clientPID;

        if ((childPID = fork()) < 0) {
            perror("Failed to fork");
            exit(EXIT_FAILURE);
        }
        if (childPID == 0) { //child process

            subServerInf.subServerPID = (int)getpid();

            #ifdef DEBUG
            fprintf(stderr, "DEBUG****subServerPID: %d\n",subServerInf.subServerPID );
            #endif
			subServerInf.isKill = false;

            if ( write(clientFifoFD,&subServerInf,sizeof(subServerInf)) == ERROR){
                fprintf(stderr, "Failed to write sub server information, %s\n",strerror(errno));
                exit(EXIT_FAILURE);
            }

			#ifdef DEBUG
            fprintf(stderr, "DEBUG****child is created write to sub server information\n" );
            #endif

            if (signal(SIGUSR2, SIGUSR2_handler) == SIG_ERR) {
                perror("SIGUSR2 install error");
            }

            while(1){

                pause();
				curtime = time(NULL);
				memset(serverLog.matrixGenerationTime,'\0',sizeof(char));
				snprintf(serverLog.matrixGenerationTime,32,"%s",ctime(&curtime));
                #ifdef DEBUG
                fprintf(stderr, "DEBUG****pass the second pause:\n");
                #endif
                startServer(serverInf,subServerInf);
				writeToLog();
            }
        }
    }
}


void startServer(struct ServerInf svr, struct SubServerInf subSvr){
    int i = 0, j = 0;

    generateMatrix(subServerInf.matrix,serverInf.matrixSize);

    #ifdef DEBUG
    fprintf(stderr, "DEBUG****clientFifoName: %s\n",clientInf.clientFifoName );
    fprintf(stderr, "DEBUG****parentPID: %ld\n",(long)getppid());
    fprintf(stderr, "DEBUG****Original matrix: \n" );

    for (i = 0; i < serverInf.matrixSize; i++) {
        for (j = 0; j < serverInf.matrixSize; j++) {
            fprintf(stderr, "%.2f  ",subServerInf.matrix[i][j] );
        }
        fprintf(stderr, "\n");
    }

    fprintf(stderr, "DEBUG****---------------\n" );
    #endif

    if (isInvertibleMatrix(subServerInf.matrix,serverInf.matrixSize)) {
        fprintf(stderr, "this matrix is invertible\n" );
    } else {
        fprintf(stderr, "this matrix isn't invertible\n" );
    }

    #ifdef DEBUG
    fprintf(stderr, "DEBUG****sizeof subSvr: %ld\n",sizeof(subSvr) );
    #endif

    if (write(clientFifoFD,&subServerInf,sizeof(subServerInf)) == ERROR) {
        fprintf(stderr, "Failed to write sub server information 2 %s\n",strerror(errno) );
        exit(EXIT_FAILURE);
    }

    #ifdef DEBUG
    fprintf(stderr, "DEBUG****matrix was sent\n");
    #endif
}

void generateMatrix(double matrix[][MM_SIZE], int matrixSize){
    int temp = 0, i = 0, j = 0;
	struct timeval t1;
	gettimeofday(&t1, NULL);

	srand(t1.tv_usec);
	temp = rand() % 100;

    for (i = 0; i < matrixSize; i++) {
        for (j = 0; j < matrixSize; j++) {
            matrix[i][j] = rand() % 100;
        }
    }
}

bool isInvertibleMatrix(){
	int i = 0, j = 0;
	double det;
    double **tempMatrix;
    tempMatrix = allocMatrix(serverInf.matrixSize);
	copyMatrix(tempMatrix,serverInf.matrixSize);
    det = determinant(tempMatrix,serverInf.matrixSize);
    freeMatrix(tempMatrix,serverInf.matrixSize);
	serverLog.det = det;

    #ifdef DEBUG
    fprintf(stderr, "DEBUG****determinant: %f\n",det);
    #endif

    if (det == 0.000000) {
        return false;
    }
	subServerInf.det = det;
    return true;
}

void copyMatrix(double **destMtrx, int size){
    int i = 0, j = 0;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            destMtrx[i][j] = subServerInf.matrix[i][j];
        }
    }
}

void freeMatrix(double **mat, int size){
    int i = 0;

    for (i = 0; i < size; i++)
        free(mat[i]);
    free(mat);
}

double **allocMatrix(int size){
    int i = 0;
    double **matrix;

    matrix = (double **)calloc(size,sizeof(double *));
    for (i = 0; i < size; i++) {
        matrix[i] = (double *)calloc(size,sizeof(double));
    }

    return matrix;
}

void SIGINT_handler(int signum){

    fprintf(stderr, "\nCaught signal %d, coming out...",signum );
    close(mainFifoFD);
    //unlink(clientInf.clientFifoName);
	subServerInf.isKill = true;
	if (write(clientFifoFD,&subServerInf,sizeof(subServerInf)) == ERROR) {
        fprintf(stderr, "Failed to write sub server information 2 %s\n",strerror(errno) );
        exit(EXIT_FAILURE);
    }

    while( waitpid(-1,NULL,WUNTRACED) != -1);

    close(mainFifoFD);
    unlink(mainFifoName);
	fclose(pServerLog);
    free(mainFifoName);
    exit(EXIT_SUCCESS);
}

void SIGUSR1_handler(int signum){
    fprintf(stderr, "\nCaught signal %d, starting client...\n",signum );
}

void SIGUSR2_handler(int signum){
    fprintf(stderr, "\nCaught signal %d, sending matrix...\n",signum );
}

//http://study-for-exam.blogspot.com.tr/2013/04/write-c-progrm-to-find-determinant-of.html
double determinant(double **mtrx,int size)
{
    int i = 0, k = 0,m = 0;
    double det = 0.0;
    double **temp;

    if (size == 2) {
        return mtrx[0][0] * mtrx[1][1] - mtrx[0][1] * mtrx[1][0];
    } else {
        temp = allocMatrix(size-1);
        for (i = 0; i < size; i++) {
            for ( k = 1; k < size; k++) {
                for (m = 0; m < size; m++) {
                    if (m < i) {
                        temp[k - 1][m] = subServerInf.matrix[k][m];
                    } else if (m > i) {
                        temp[k - 1][m - 1] = subServerInf.matrix[k][m];
                    }
                }
            }
            det += mtrx[0][i] * pow(-1,i) * determinant(temp,size - 1);
        }
        //freeMatrix(temp,size-1);
        return det;
    }
}

void writeToLog(){
	fprintf(pServerLog, "***********************************************************\n");
	fprintf(pServerLog, "Client PID of %d, ",serverLog.clientPID );
	fprintf(pServerLog, "2%d*2%d matrix ",serverLog.matrixSize/2,serverLog.matrixSize/2);
	fprintf(pServerLog, "was generated at the time of %s, ",serverLog.matrixGenerationTime);
	fprintf(pServerLog, "determinant is %.3f\n",serverLog.det );
	fprintf(pServerLog, "***********************************************************\n");
}

ssize_t r_read(int fd, void *buf, size_t size) {
	ssize_t retval;
	while (retval = read(fd, buf, size), retval == -1 && errno == EINTR) ;

	return retval;
}
