/*
 * Midterm_121044029_Mehmet_Gurol_Cay
 *
 * showResults.c
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

#include "showResults.h"

int main(int argc, char const *argv[]) {

    if (signal(SIGINT, SIGINT_handler) == SIG_ERR){
        perror("SIGINT install error");
        return 1;
    }
    
    if ( (mkfifo(SHOWRESULTS_FIFO,FIFO_PERMS) == -1) && (errno != EEXIST) ) {
        fprintf(stderr, "showResults failed to create the %s:%s\n",SHOWRESULTS_FIFO,strerror(errno) );
        return ERROR;
    }

    if ((showResultFifoFD = open(SHOWRESULTS_FIFO,O_RDWR)) == ERROR) {
        fprintf(stderr, "showResults failed to open its %s: %s\n",SHOWRESULTS_FIFO, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if ((pLogOutput = fopen(SHOWRESULTS_LOG,"w")) == NULL) {
        perror("Failed to open showResults.log file");
        return ERROR;
    }

    while(1){
        if(ERROR == r_read(showResultFifoFD,&resultOfMatrices,sizeof(resultOfMatrices))){
            perror("Failed to read from showResults.fifo");
            exit(EXIT_FAILURE);
        }
        #ifdef DEBUG
        fprintf(stderr, "i read\n" );
        #endif

        if (resultOfMatrices.isKill) {
            break;
        }
        fprintf(stderr, "clientPID: %d, DetOfShifted: %.3f, DetOfConv: %.3f\n",
                                                    resultOfMatrices.clientPID,
                                                    resultOfMatrices.detShifted,
                                            resultOfMatrices.detConvolution );
        writeToLog();
    }

    close(showResultFifoFD);
    fclose(pLogOutput);
    unlink(SHOWRESULTS_FIFO);
    return 0;
}

void writeToLog(){
    fprintf(pLogOutput, "*******************************************************\n");
    fprintf(pLogOutput, "OrgDet: %.3f, clientPID: %d",resultOfMatrices.orgDet,resultOfMatrices.clientPID );
    fprintf(pLogOutput, "DetOfShifted: %.3f, calculatedTime: %d",resultOfMatrices.detShifted,resultOfMatrices.calcTimeOfShifted );
    fprintf(pLogOutput, "DetOfConv: %.3f, calculatedTime: %d\n",resultOfMatrices.detConvolution,resultOfMatrices.calcTimeOfConv );
    fprintf(pLogOutput, "*******************************************************\n" );
}

ssize_t r_read(int fd, void *buf, size_t size) {
	ssize_t retval;
	while (retval = read(fd, buf, size), retval == -1 && errno == EINTR) ;

	return retval;
}
void SIGINT_handler(int signum){
    close(showResultFifoFD);
    fclose(pLogOutput);
    unlink(SHOWRESULTS_FIFO);
    exit(EXIT_SUCCESS);
}
