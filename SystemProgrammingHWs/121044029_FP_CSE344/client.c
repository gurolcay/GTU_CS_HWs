//
// Created by gurol on 31.05.2018.
//

#include "client.h"


int main(int argc, const char *argv[]) {
	struct Client client;
	struct timeval t1, t2;
	double elapsedTime;
	if (argc != 6) {
		fprintf(stdout,
		        RED"Usage: %s <client_name> <priority> <degree> <server_ip_address> <server_port_number>\n"RESET,
		        argv[0]);
		return -1;
	}

	memset(serverIP, '\0', MINSTRSIZE);

	strcpy(client.clientName, argv[1]);
	client.priority = argv[2][0];
	client.degree = (int) strtol(argv[3], NULL, 10);
	strcpy(serverIP, argv[4]);
	serverPortNumber = (int) strtol(argv[5], NULL, 10);

	if (-1 == createClientSocket()) {
		exit(EXIT_FAILURE);
	}

	gettimeofday(&t1, NULL);

	if (-1 == connectToServer()) {
		close(serverSock);
		exit(EXIT_FAILURE);
	}

	fprintf(stdout, YEL"Client, %s is requesting %c %d from %s:%d\n"RESET, client.clientName, client.priority,
	        client.degree, serverIP, serverPortNumber);
	ssize_t sentBytes = write(serverSock, (void *) &client, sizeof(struct Client));

#ifdef DEBUG
	fprintf(stdout, "sent bytes: %d\nexpected bytes: %d\n", (int) sentBytes, (int) sizeof(client));
#endif

	if (sentBytes == -1) {
		fprintf(stderr, RED"Failed to sent all message, sent bytes: %d: %s"RESET, (int) sentBytes, strerror(errno));
		close(serverSock);
		exit(EXIT_FAILURE);
	}

	struct Result result;
	ssize_t status = read(serverSock, (void *) &result, sizeof(struct Client));

	if (status == -1) {
		fprintf(stderr, RED"Failed to read from server: %s", strerror(errno));
		close(serverSock);
		exit(EXIT_FAILURE);
	}

	if (result.shutdown){
		fprintf(stderr, RED"SERVER SHUTDOWN\n"RESET);
		close(serverSock);
		exit(EXIT_FAILURE);
	}

	if (result.error) {
		fprintf(stderr, RED"NO PROVIDER IS AVAILABLE\n"RESET);
		close(serverSock);
		exit(EXIT_FAILURE);
	}

	gettimeofday(&t2, NULL);
	elapsedTime = (t2.tv_sec - t1.tv_sec) ;
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0;

	fprintf(stdout,
	        GRN"%s's task completed by %s in  %f seconds, cos(%d)=%lf, cost is %d, total time spent %6.2f seconds\n"RESET,
	        client.clientName, result.providerName, result.taskCompTime, client.degree, result.result, result.cost,
	        elapsedTime);

	close(serverSock);
	return 0;
}

int createClientSocket() {

	fprintf(stderr, "Creating socket...\n");
	if ((serverSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Failed to create socket");
		return -1;
	}

	memset(&serverAddr, 0, sizeof(struct sockaddr_in));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(serverIP);
	serverAddr.sin_port = htons(serverPortNumber);
	fprintf(stderr, "Server Port: %d\n", serverPortNumber);

	return 1;
}

int connectToServer() {
	fprintf(stderr, "....\n");
	if (connect(serverSock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
		perror("Connection error");
		return -1;
	}

	return 1;
}