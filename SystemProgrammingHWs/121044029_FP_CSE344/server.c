#include "server.h"

int main(int argc, char const *argv[]) {
	struct sigaction sact;
	sigset_t block_mask, orig_mask;

	if (argc != 4) {
		fprintf(stderr, RED"Usage: %s <port_number> <provide_file_name> <log_file_name>"RESET, argv[1]);
		exit(EXIT_FAILURE);
	}
	memset(&sact,0, sizeof(sact));
	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGINT);
	sigaddset(&block_mask, SIGTERM);
	sact.sa_flags = 0;
	sact.sa_handler = &signalHandler;


	pLogFile = fopen(argv[3], "w+");
	if (pLogFile == NULL) {
		fprintf(stderr, RED"Failed to open log file: %s :%s\n"RESET, argv[3], strerror(errno));
		fprintf(pLogFile, "Failed to open log file: %s :%s\n", argv[3], strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (sigaction(SIGINT, &sact, NULL) != 0) {
		fprintf(stderr, RED"SIGINT sigaction error: %s\n", strerror(errno));
		fprintf(pLogFile, "SIGINT sigaction error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (sigaction(SIGTERM, &sact, NULL) != 0) {
		fprintf(stderr, RED"SIGTERM sigaction error: %s\n", strerror(errno));
		fprintf(pLogFile, "SIGTERM sigaction error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	serverIPAddr = (char *) calloc(MINSTRSIZE, sizeof(char));

	selectDevice();
	puts(YEL"\n****************************************************");
	fprintf(stderr, "Server port number: %s\n", argv[1]);
	fprintf(stderr, "server IP address: %s\n", serverIPAddr);
	puts("****************************************************"RESET);

	fprintf(pLogFile, "Server port number: %s\n", argv[1]);
	fprintf(pLogFile, "server IP address: %s\n", serverIPAddr);

	fprintf(stderr, GRN"Server start with these parameters: \n");
	fprintf(stderr, "%s %s %s %s", argv[0], argv[1], argv[2], argv[3]);
	fprintf(pLogFile, "Server start with these parameters: \n");
	fprintf(pLogFile, "%s %s %s %s", argv[0], argv[1], argv[2], argv[3]);

	fprintf(stderr, "Logs keep at %s\n", argv[3]);
	fprintf(pLogFile, "Logs keep at %s\n", argv[3]);
	parseDataFile(argv[2]);

	fprintf(stdout, "%d provider threads created\n", providerNum);
	fprintf(pLogFile, "%d provider threads created\n", providerNum);

	fprintf(stdout, "Name \t\tPerformance\tPrice\t\tDuration\n");
	fprintf(pLogFile, "Name \t\tPerformance\tPrice\t\tDuration\n");

	for (int i = 0; i < providerNum; ++i) {
		fprintf(stderr, "%s\t\t%d\t\t%d\t\t%d\n", providers[i].providerName, providers[i].performance,
		        providers[i].price, providers[i].duration);
		fprintf(pLogFile, "%s\t\t%d\t\t%d\t\t%d\n", providers[i].providerName, providers[i].performance,
		        providers[i].price, providers[i].duration);
	}
	fprintf(stdout, ""RESET);

	providersThID = (pthread_t *) calloc(providerNum, sizeof(pthread_t));

	//	TODO: bu degisken yuzunden memory leak olabilir kontrol et!!!!
	int *providerID;

	providerID = (int *) calloc(providerNum, sizeof(int));
	for (int j = 0; j < providerNum; ++j) {
		providerID[j] = j;
		int status = pthread_create(&providersThID[j], NULL, ThreadProvider, (void *) &providerID[j]);
		if (status != 0) {
			fprintf(stderr, RED"Failed to create thread: %s"RESET, strerror(errno));
			fprintf(pLogFile, "Failed to create thread: %s", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	int status = pthread_create(&durationTimeCheckerThID, NULL, ThreadDurationTimeChecker, (void *) NULL);
	if (status != 0) {
		fprintf(stderr, RED"Failed to cerate duration time checker thread: %s\n"RESET, strerror(errno));
		fprintf(pLogFile, "Failed to cerate duration time checker thread: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	serverSocketNumber = (unsigned short) strtol(argv[1], NULL, 10);

	startServer();

	pthread_cancel(connectionHandleThID);

	for (int l = 0; l < providerNum; ++l) {
		pthread_cancel(providersThID[l]);
	}

	struct Result result;
	result.shutdown = true;
	for (int k = 0; k < providerNum; ++k) {
		for (int i = 0; i < 2; ++i) {
			if (providers[k].providerQueue[i].clientSocket != -1){
				write(providers[k].providerQueue[i].clientSocket, (void *)&result, sizeof(result));
			}
		}
	}

	pthread_cancel(durationTimeCheckerThID);
	pthread_cancel(connectionHandleThID);
	free(providersThID);
	free(serverIPAddr);
	free(providerID);
	free(providers);
	fclose(pLogFile);
	return 0;
}

void parseDataFile(const char *fileName) {
	FILE *pInputFile;
	char line[BUFFSIZE], *buff;
	int i = 0;

	pInputFile = fopen(fileName, "r");

	if (pInputFile == NULL) {
		fprintf(stderr, RED"Failed to open %s: %s\n"RESET, fileName, strerror(errno));
		fprintf(pLogFile, "Failed to open %s: %s\n", fileName, strerror(errno));
		close(serverSock);
		exit(EXIT_FAILURE);
	}

	fgets(line, BUFFSIZE, pInputFile);
	while (!feof(pInputFile)) {
		fgets(line, BUFFSIZE, pInputFile);
		++providerNum;
	}

	fclose(pInputFile);

	providers = (struct Provider *) calloc(providerNum, sizeof(struct Provider));

	if (providers == NULL) {
		fprintf(stderr, "failed to calloc %s", strerror(errno));
		fprintf(pLogFile, "failed to calloc %s", strerror(errno));
		close(serverSock);
		exit(EXIT_FAILURE);
	}

	pInputFile = fopen(fileName, "r");

	fgets(line, BUFFSIZE, pInputFile);

	while (!feof(pInputFile)) {
		fgets(line, BUFFSIZE, pInputFile);

		buff = strtok(line, " ");
		strcpy(providers[i].providerName, buff);

		buff = strtok(NULL, " ");
		providers[i].performance = (int) strtol(buff, NULL, 10);

		buff = strtok(NULL, " ");
		providers[i].price = (int) strtol(buff, NULL, 10);

		buff = strtok(NULL, " ");
		providers[i].duration = (int) strtol(buff, NULL, 10);

		providers[i].countQueue = 0;
		providers[i].threadID = 0;
		providers[i].front = 0;
		providers[i].rear = -1;
		providers[i].isProviderRunning = false;

		memset(&(providers[i].providerQueue[0]), 0, sizeof(providers[i].providerQueue[0]));
		memset(&(providers[i].providerQueue[1]), 0, sizeof(providers[i].providerQueue[1]));
		providers[i].providerQueue[0].clientSocket = -1;
		providers[i].providerQueue[1].clientSocket = -1;

		pthread_cond_init(&(providers[i].condVarProvider), NULL);
		pthread_mutex_init(&(providers[i].condVarLocProvider), NULL);

		++i;
		if (i == providerNum)
			break;

	}

	aliveProviderTh = i + 1;

	fclose(pInputFile);
}

void signalHandler(int signum) {
	switch (signum) {
		case SIGINT:
			fprintf(stderr, WHT"Catched SIGINT signal. Exiting the program... \n"RESET);
			fprintf(pLogFile, "Catched SIGINT signal. Exiting the program... \n");
			isRunning = false;
			pthread_cond_signal(&condVar);
			break;
		case SIGTERM:
			fprintf(stderr, WHT"Catched SIGTERM signal. Exiting the program... \n"RESET);
			fprintf(pLogFile, "Catched SIGTERM signal. Exiting the program... \n");
			isRunning = false;
			pthread_cond_signal(&condVar);
			break;
		default:
			break;
	}

}

int startServer() {
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(serverSocketNumber);


	if ((serverSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, RED"Failed to create socket: %s\n"RESET, strerror(errno));
		fprintf(pLogFile, "Failed to create socket: %s\n", strerror(errno));
		return -1;
	}

	if (bind(serverSock, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
		fprintf(stderr, RED"Failed to bind socket to port: %s\n"RESET, strerror(errno));
		fprintf(pLogFile, "Failed to bind socket to port: %s\n", strerror(errno));
		return -1;
	}

	if (listen(serverSock, 10) == -1) {
		fprintf(stderr, RED"Failed to listen socket: %s"RESET, strerror(errno));
		fprintf(pLogFile, "Failed to listen socket: %s", strerror(errno));
		return -1;
	}

	int status = pthread_create(&connectionHandleThID, NULL, ThreadConnectionHandler, NULL);

	if (status == -1) {
		fprintf(stderr, RED"Failed to create connection handler thread: %s\n"RESET, strerror(errno));
		fprintf(pLogFile, "Failed to create connection handler thread: %s\n", strerror(errno));
		isRunning = false;
	}

	while (isRunning) {
		pthread_cond_wait(&condVar, &condVarLoc);
	}

	close(serverSock);
	return isRunning;
}

int acceptClient() {
	unsigned int clientLen;

	clientLen = sizeof(struct sockaddr_in);

	if ((clientSock = accept(serverSock, (struct sockaddr *) &clientAddress, &clientLen)) < 0) {
		fprintf(stderr,"Failed to accept client");
		fprintf(pLogFile,"Failed to accept client");
		return -1;
	}

	if (clientSock < 0) {
		fprintf(stderr,"Accept failed: \n");
		fprintf(pLogFile,"Accept failed: \n");
		return -1;
	}

#ifdef DEBUG
	fprintf(stderr, "DEBUG: acceptClient: serverSock: %d\n",serverSock );
	fprintf(stderr, "DEBUG: acceptClient: clientSock: %d\n",clientSock );
#endif

	return clientSock;
}

void *ThreadDurationTimeChecker(void *var) {
	struct timeval t1, t2;
	double elapsedTime = 0.0;
	int *cancelledThreadIDs;
	gettimeofday(&t1, NULL);


//	pthread_detach(pthread_self());

	cancelledThreadIDs = (int *) calloc(providerNum, sizeof(int));

	for (int k = 0; k < providerNum; ++k) {
		cancelledThreadIDs[k] = -1;
	}

	while (isRunning) {
		gettimeofday(&t2, NULL);
		elapsedTime = (t2.tv_sec - t1.tv_sec);
		elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0;

		for (int i = 0; i < providerNum; ++i) {
			if (cancelledThreadIDs[i] == -1) {
				if (providers[i].duration <= elapsedTime) {
					if (!(providers[i].isProviderRunning)) {

						pthread_cancel(providersThID[i]);
						cancelledThreadIDs[i] = 1;

						pthread_mutex_lock(&condVarLocTh);
						fprintf(stderr, YEL"Provider %s logouting \n"RESET, providers[i].providerName);
						fprintf(pLogFile, "Provider %s logouting \n", providers[i].providerName);

						--aliveProviderTh;
						pthread_mutex_unlock(&condVarLocTh);
					}
				}
			}
		}
		sleep(10);
	}

	free(cancelledThreadIDs);
	return NULL;
}

void *ThreadConnectionHandler(void *var) {


	while (isRunning) {
		int clientSocket = acceptClient();
		pthread_t thread_id;

		if (-1 == clientSocket) {
			isRunning = false;
		}

		if (aliveProviderTh <= 0) {
			fprintf(stderr, RED"NO PROVIDER IS AVAILABLE\n"RESET);
			fprintf(pLogFile, "NO PROVIDER IS AVAILABLE\n");
			struct Result result;
			memset(&result, 0, sizeof(result));
			result.error = true;
			write(clientSocket, (void *) &result, sizeof(result));
			isRunning = false;
		}

		pthread_mutex_lock(&condVarLocTh);
		++aliveManagerTh;
		pthread_mutex_unlock(&condVarLocTh);

#ifdef DEBUG
		fprintf(stderr, "DEBUG: ThreadConnectionHandler: Client Sock: %d\n",clientSock);
#endif

		int status = pthread_create(&thread_id, NULL, ThreadManager, (void *) &clientSocket);
		if (status != 0) {
			fprintf(stderr, RED"Failed to create thread: %s\n"RESET, strerror(errno));
			fprintf(pLogFile, "Failed to create thread: %s\n", strerror(errno));
			isRunning = false;
			--aliveManagerTh;
		}
	}

	return NULL;
}

void *ThreadProvider(void *id) {
	int *providerID = (int *) id;
	bool isFinishDurationTime = false;
	struct timespec ts;
	struct timeval startTime, endTime;
	struct timeval t1, t2;
	double elapsedTime;
	struct Result result;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	srand((time_t) ts.tv_nsec);

#ifdef DEBUG
	fprintf(stderr, RED"DEBUG: ThreadProvider: id: %d\n"RESET, *providerID);
#endif
	gettimeofday(&startTime, NULL);

	while (!isFinishDurationTime) {
		fprintf(stderr, BLU"Provider %s is waiting for a task\n"RESET, providers[*providerID].providerName);
		fprintf(pLogFile, "Provider %s is waiting for a task\n", providers[*providerID].providerName);

		while (isEmpty(*providerID)) {
			providers[*providerID].isProviderRunning = false;
			pthread_cond_wait(&(providers[*providerID].condVarProvider), &(providers[*providerID].condVarLocProvider));
		}

		while (!isEmpty(*providerID)) {
			providers[*providerID].isProviderRunning = true;
			fprintf(stderr, BLU"Provider %s is processing task number %d: %d\n"RESET, providers[*providerID].providerName,
			        providers[*providerID].countQueue,
			        providers[*providerID].providerQueue[providers[*providerID].front].degree);
			fprintf(pLogFile, "Provider %s is processing task number %d: %d\n", providers[*providerID].providerName,
			        providers[*providerID].countQueue,
			        providers[*providerID].providerQueue[providers[*providerID].front].degree);

			gettimeofday(&t1, NULL);
			result.result = calcCosDeg(providers[*providerID].providerQueue[providers[*providerID].front].degree);


			int rand_v;
			rand_v = rand() % 15;
			while (!(5 < rand_v && rand_v < 15)) {
				rand_v = rand() % 15;
			}

			sleep(rand_v);

			gettimeofday(&t2, NULL);
			strcpy(result.providerName, providers[*providerID].providerName);
			result.cost = providers[*providerID].price;
			result.taskCompTime = (t2.tv_sec - t1.tv_sec);
			result.taskCompTime += (t2.tv_usec - t1.tv_usec) / 1000000.0;
			result.error = false;
			result.shutdown = false;

			struct Client client;
			client = peek(*providerID);


			write(client.clientSocket, (void *) &result, sizeof(result));


			fprintf(stderr, BLU"Provider %s completed task number %d: cos(%d)=%lf in %f seconds\n"RESET,
			        providers[*providerID].providerName, providers[*providerID].countQueue,
			        providers[*providerID].providerQueue[providers[*providerID].front].degree,
			        result.result,
			        result.taskCompTime);

			fprintf(pLogFile, "Provider %s completed task number %d: cos(%d)=%lf in %f seconds\n",
			        providers[*providerID].providerName, providers[*providerID].countQueue,
			        providers[*providerID].providerQueue[providers[*providerID].front].degree,
			        result.result,
			        result.taskCompTime);

			pthread_mutex_lock(&condVarLocTh);
			removeData(*providerID);
			pthread_mutex_unlock(&condVarLocTh);
		}

		gettimeofday(&endTime, NULL);

		elapsedTime = (endTime.tv_sec - startTime.tv_sec);
		elapsedTime += (endTime.tv_usec - startTime.tv_usec) / 1000000.0;

		if (elapsedTime >= providers[*providerID].duration) {
			isFinishDurationTime = true;
		}

	}

	pthread_mutex_lock(&condVarLocTh);
	--aliveProviderTh;
	pthread_mutex_unlock(&condVarLocTh);

	return NULL;
}

double calcCosDeg(int degree) {
	double rad = 0.0, result = 0.0;
	rad = (PI / 180) * degree;

	for (int i = 0; i < DBL_DIG; ++i)
		result += pow(-1.0, i) / factorial(2 * i) * pow(rad, 2 * i);

	return result;
}

long int factorial(int n) {
	if (n <= 1) {
		return 1;
	} else {
		return n * factorial(n - 1);
	}
}

void *ThreadManager(void *clientSock) {
	int *clntSock = (int *) clientSock;
	ssize_t readBytes;
	struct Client client;
	pthread_t threadID;

	threadID = pthread_self();
	pthread_detach(threadID);

	readBytes = read(*clntSock, (void *) &client, sizeof(struct Client));
	if (readBytes == -1) {
		fprintf(stderr, RED"Failed to read from socket: %s\n"RESET, strerror(errno));
		fprintf(pLogFile, "Failed to read from socket: %s\n", strerror(errno));
		pthread_exit(NULL);
	}

	client.clientSocket = *clntSock;
#ifdef DEBUG
	fprintf(stdout, GRN"DEBUG: ThreadManager: client:");
	fprintf(stdout, "DEBUG: ThreadManager: priority: %c\n", client.priority);
	fprintf(stdout, "DEBUG: ThreadManager: degree: %d\n", client.degree);
	fprintf(stdout, "DEBUG: ThreadManager: clientName: %s\n", client.clientName);
	fprintf(stdout, "DEBUG: ThreadManager: clientSocket: %d\n"RESET, client.clientSocket);

#endif

	bool isFoundBest = false;
	int bestProviderIdx = 0;
	if (client.priority == 'C') {
		for (int i = 0; i < providerNum; ++i) {
			if (providers[i].price < providers[bestProviderIdx].price && providers[i].countQueue < 2) {
				bestProviderIdx = i;
				isFoundBest = true;
			}
		}
	} else if (client.priority == 'Q') {
		for (int i = 0; i < providerNum; ++i) {
			if (providers[i].performance > providers[bestProviderIdx].performance && providers[i].countQueue < 2) {
				bestProviderIdx = i;
				isFoundBest = true;
			}
		}
	} else if (client.priority == 'T') {
		for (int i = 0; i < providerNum; ++i) {
			if (isEmpty(i)) {
				bestProviderIdx = i;
				isFoundBest = true;
				break;
			}
		}
	}

	if (!isFoundBest) {
		for (int i = 0; i < providerNum; ++i) {
			if (isEmpty(i)) {
				bestProviderIdx = i;
				isFoundBest = true;
			}
		}
	}

	if (!isFoundBest) {
		for (int i = 0; i < providerNum; ++i) {
			if (providers[i].countQueue == 1) {
				bestProviderIdx = i;
				isFoundBest = true;
			}
		}
	}

	if (!isFoundBest) {
		while (!isFoundBest) {
			for (int i = 0; i < providerNum; ++i) {
				if (providers[i].countQueue == 0 || providers[i].countQueue == 1) {
					bestProviderIdx = i;
					isFoundBest = true;
				}
			}
		}
	}

	pthread_mutex_lock(&condVarLocTh);
	insert(bestProviderIdx, client);
	--aliveManagerTh;
	pthread_mutex_unlock(&condVarLocTh);

	pthread_cond_signal(&(providers[bestProviderIdx].condVarProvider));


	return (void *) 1;
}

void selectDevice() {
	struct ifaddrs *ifap, *ifa;
	struct sockaddr_in *sa;
	char **addr;
	int connNum = 0;
	int i = 0, j = 0;

	getifaddrs(&ifap);
	for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr->sa_family == AF_INET) {
			++connNum;
		}
	}

	addr = (char **) malloc(connNum * sizeof(char *));
	for (i = 0; i < connNum; ++i) {
		addr[i] = (char *) malloc(MINSTRSIZE * sizeof(char));
	}

	i = 0;
	getifaddrs(&ifap);
	puts("****************************************************");
	for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr->sa_family == AF_INET) {
			sa = (struct sockaddr_in *) ifa->ifa_addr;
			strcpy(addr[i], inet_ntoa(sa->sin_addr));
			printf("Interface:[%d] %s          Address: %s\n", i, ifa->ifa_name, addr[i]);
			++i;
		}
	}
	puts("****************************************************");

	fprintf(stdout, "Select connection device: ");
	fscanf(stdin, "%d", &i);
	strcpy(serverIPAddr, addr[i]);
//	fprintf(stdout, "server IP address: %s-\n", serverIPAddr);
	j = 0;
	getifaddrs(&ifap);
	for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr->sa_family == AF_INET) {
			sa = (struct sockaddr_in *) ifa->ifa_addr;
			if (j == i) {
				serverAddress.sin_addr = ((struct sockaddr_in *) &ifa->ifa_addr)->sin_addr;
			}
			++j;
		}
	}


	freeifaddrs(ifap);

	for (i = 0; i < connNum; ++i)
		free(addr[i]);
	free(addr);
}

struct Client peek(int providerNum) {
	struct Client data;

	data.degree = providers[providerNum].providerQueue[providers[providerNum].front].degree;
	data.priority = providers[providerNum].providerQueue[providers[providerNum].front].priority;
	data.clientSocket = providers[providerNum].providerQueue[providers[providerNum].front].clientSocket;
	strcpy(data.clientName, providers[providerNum].providerQueue[providers[providerNum].front].clientName);

#ifdef DEBUG
	fprintf(stderr, CYN"DEBUG: peek : degree: %d\n"RESET, data.degree);
	fprintf(stderr, CYN"DEBUG: peek : clientSocket: %d\n"RESET, data.clientSocket);
	fprintf(stderr, CYN"DEBUG: peek : clientName: %s\n"RESET, data.clientName);
	fprintf(stderr, CYN"DEBUG: peek : pri: %c\n"RESET, data.priority);
	fprintf(stderr, CYN"DEBUG: peek : countQueue: %d\n"RESET, providers[providerNum].countQueue);
#endif

	return data;
}

bool isEmpty(int providerNum) {
	return providers[providerNum].countQueue == 0;
}

bool isFull(int providerNum) {
	return providers[providerNum].countQueue == MAXQUEUENUM;
}

int size(int providerNum) {
	return providers[providerNum].countQueue;
}

void insert(int providerNum, struct Client newClient) {
	if (!isFull(providerNum)) {
		if (providers[providerNum].rear == MAXQUEUENUM - 1) {
			providers[providerNum].rear = -1;
		}
#ifdef DEBUG
		fprintf(stderr, MAG"DEBUG: insert: provider num: %d\n"RESET, providerNum);
#endif
		++(providers[providerNum].rear);
		providers[providerNum].providerQueue[providers[providerNum].rear].degree = newClient.degree;
		providers[providerNum].providerQueue[providers[providerNum].rear].clientSocket = newClient.clientSocket;
		providers[providerNum].providerQueue[providers[providerNum].rear].priority = newClient.priority;
		strcpy(providers[providerNum].providerQueue[providers[providerNum].rear].clientName, newClient.clientName);
		++(providers[providerNum].countQueue);

#ifdef DEBUG
		fprintf(stderr, MAG"DEBUG: insert: degree: %d\n"RESET,
				providers[providerNum].providerQueue[providers[providerNum].rear].degree);
		fprintf(stderr, MAG"DEBUG: insert: clientSocket: %d\n"RESET,
				providers[providerNum].providerQueue[providers[providerNum].rear].clientSocket);
		fprintf(stderr, MAG"DEBUG: insert: clientName: %s\n"RESET,
				providers[providerNum].providerQueue[providers[providerNum].rear].clientName);
		fprintf(stderr, MAG"DEBUG: insert: pri: %c\n"RESET,
				providers[providerNum].providerQueue[providers[providerNum].rear].priority);
		fprintf(stderr, MAG"DEBUG: insert: countQueue: %d\n"RESET, providers[providerNum].countQueue);
#endif

	}
}

struct Client removeData(int providerNum) {
	struct Client data;

	strcpy(data.clientName, providers[providerNum].providerQueue[providers[providerNum].front].clientName);
	data.priority = providers[providerNum].providerQueue[providers[providerNum].front].priority;
	data.degree = providers[providerNum].providerQueue[providers[providerNum].front].degree;
	data.clientSocket = providers[providerNum].providerQueue[providers[providerNum].front].clientSocket;
	++providers[providerNum].front;
	if (providers[providerNum].front == MAXQUEUENUM) {
		providers[providerNum].front = 0;
	}
	--providers[providerNum].countQueue;

	return data;
}