#include <time.h>
#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <semaphore.h>
#include <stdint.h>

// Color definition for terminal outputs
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"
// end of color definition

#define THREADHANDLERNUM    10
#define BUFFSIZE            128
#define FLOWER              1
#define CLIENT              2
#define ERROR               -1

struct Florist {
	int x;
	int y;
	double speed;
	char flower1[16];
	char flower2[16];
	char flower3[16];
	char florist_name[16];
};

struct Client {
	int x;
	int y;
	char client_name[16];
	char flower[16];
	double distance;
};

struct Florists {
	struct Florist florist;
};

struct ThreadQueue {
	struct Client client;
};

struct ThreadReturnValue {
	char florist_name[16];
	char client_name[16];
	char flower[16];
	int delta;
};

struct SalesStatistics {
	int florist1Sales;
	int florist2Sales;
	int florist3Sales;

	int florist1TotalTime;
	int florist2TotalTime;
	int florist3TotalTime;
};


char *INPUTFILENAME;

static int front1 = 0;
static int rear1 = -1;
static int front2 = 0;
static int rear2 = -1;
static int front3 = 0;
static int rear3 = -1;
static int maxQueueNum = 10;

volatile int countQueue1 = 0;
volatile int countQueue2 = 0;
volatile int countQueue3 = 0;

struct Florists *florists;

struct ThreadQueue *threadQueue1;
struct ThreadQueue *threadQueue2;
struct ThreadQueue *threadQueue3;

struct SalesStatistics salesStatistics;

pthread_t *florist_th1;
pthread_t *florist_th2;
pthread_t *florist_th3;

volatile int florist1Count = 0;
volatile int florist2Count = 0;
volatile int florist3Count = 0;

pthread_mutex_t condVarLocTh1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condVarLocTh2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condVarLocTh3 = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t condVarTh1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t condVarTh2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t condVarTh3 = PTHREAD_COND_INITIALIZER;


void *ThreadFlorist1(void *clientNum);

void *ThreadFlorist2(void *clientNum);

void *ThreadFlorist3(void *clientNum);

int parseline(char *line, struct Florist *florist, struct Client *client, int mode);

int calcEuclideanDist(struct Client client);

void insert(int floristNum, struct Client newClient);

struct Client removeData(int floristNum);

struct Client peek(int floristNum);

bool isEmpty(int floristNum);

bool isFull(int floristNum);

int size(int floristNum);

double calcDist(int floristX, int floristY, int clientX, int clientY);

int main(int argc, char const *argv[]) {
	FILE *pInputFile;
	struct Florist florist;
	struct Client client;

	int mode = 1;
	int index = 0;
	char *line = calloc(BUFFSIZE, sizeof(char));


	if (argc != 2) {
		fprintf(stdout, GRN "Usage: %s <input_file_name>\n" RESET, argv[0]);
		fprintf(stdout, RED "The input file must be in the folder where the program is located.\n" RESET);
		exit(EXIT_FAILURE);
	}

	INPUTFILENAME = (char *) calloc(strlen(argv[1]), sizeof(char));
	strcpy(INPUTFILENAME, argv[1]);

	salesStatistics.florist1Sales = 0;
	salesStatistics.florist2Sales = 0;
	salesStatistics.florist3Sales = 0;

	salesStatistics.florist1TotalTime = 0;
	salesStatistics.florist2TotalTime = 0;
	salesStatistics.florist3TotalTime = 0;

	memset(&florist, 0, sizeof(florist));
	memset(&client, 0, sizeof(client));

	pInputFile = fopen(INPUTFILENAME, "r");

	if (pInputFile == NULL) {
		fprintf(stderr, "Failed to open %s: %s\n", INPUTFILENAME, strerror(errno));
		exit(EXIT_FAILURE);
	}


	florists = (struct Florists *) calloc(3, sizeof(struct Florist));
	florist_th1 = (pthread_t *) calloc(THREADHANDLERNUM, sizeof(pthread_t));
	florist_th2 = (pthread_t *) calloc(THREADHANDLERNUM, sizeof(pthread_t));
	florist_th3 = (pthread_t *) calloc(THREADHANDLERNUM, sizeof(pthread_t));
	threadQueue1 = (struct ThreadQueue *) calloc(maxQueueNum, sizeof(struct ThreadQueue));
	threadQueue2 = (struct ThreadQueue *) calloc(maxQueueNum, sizeof(struct ThreadQueue));
	threadQueue3 = (struct ThreadQueue *) calloc(maxQueueNum, sizeof(struct ThreadQueue));

	fprintf(stdout, "Florist application initializing from file: %s\n", INPUTFILENAME);

	while (strcmp(line, "\n") != 0) {
		fgets(line, BUFFSIZE, pInputFile);

		int status = parseline(line, &florist, &client, mode);
#ifdef DEBUG
		fprintf(stderr, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		fprintf(stderr, "x: %d",florist.x);
		fprintf(stderr, "y: %d",florist.y);
		fprintf(stderr, "speed: %f",florist.speed);
		fprintf(stderr, "flower1 %s",florist.flower1);
		fprintf(stderr, "flower2 %s",florist.flower2);
		fprintf(stderr, "flower3 %s",florist.flower3);
		fprintf(stderr, "name %s",florist.florist_name);
		fprintf(stderr, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
#endif
		if (status == FLOWER) {
			if (index < 3) {
				florists[index].florist.x = florist.x;
				florists[index].florist.y = florist.y;
				florists[index].florist.speed = florist.speed;

				strcpy(florists[index].florist.flower1, florist.flower1);
				strcpy(florists[index].florist.flower2, florist.flower2);
				strcpy(florists[index].florist.flower3, florist.flower3);
				strcpy(florists[index].florist.florist_name, florist.florist_name);

				index++;
			}
#ifdef DEBUG
			fprintf(stderr, "name: %s\n", florist.florist_name);
			fprintf(stderr, "x: %d\n", florist.x);
			fprintf(stderr, "y: %d\n", florist.y);
			fprintf(stderr, "speed: %f\n", florist.speed);
			fprintf(stderr, "flower1: %s\n", florist.flower1);
			fprintf(stderr, "flower2: %s\n", florist.flower2);
			fprintf(stderr, "flower3: %s\n", florist.flower3);
#endif
		}
	}


	fprintf(stdout, "3 florists have been created\n");
	fprintf(stdout, RED "Processing requests\n\n" RESET);

	mode = 2;
	struct timespec requestTime;
	while (!feof(pInputFile)) {

		fgets(line, BUFFSIZE, pInputFile);
		int status = parseline(line, &florist, &client, mode);

		if (status == CLIENT) {
			int floristNum = calcEuclideanDist(client);

#ifdef DEBUG
			fprintf(stderr, "floristNum: %d\n", floristNum);
#endif

			if (floristNum == 0) {
				client.distance = calcDist(florists[0].florist.x, florists[0].florist.y, client.x, client.y);
#ifdef DEBUG
				fprintf(stderr, "Insert client to queue 1\n");
#endif
				insert(floristNum, client);

#ifdef DEBUG
				struct Client _cl;
				_cl = peek(floristNum);

				fprintf(stderr, "--------------------------------------------------\n");
				fprintf(stderr, "name: %s\n", _cl.client_name);
				fprintf(stderr, "flower: %s\n", _cl.flower);
				fprintf(stderr, "x: %d\n", _cl.x);
				fprintf(stderr, "y: %d\n", _cl.y);
				fprintf(stderr, "--------------------------------------------------\n");
#endif

			} else if (floristNum == 1) {
				client.distance = calcDist(florists[1].florist.x, florists[1].florist.y, client.x, client.y);

#ifdef DEBUG
				fprintf(stderr, "Insert client to queue 2\n");
#endif
				insert(floristNum, client);

#ifdef DEBUG
				struct Client _cl;
				_cl = peek(floristNum);

				fprintf(stderr, "--------------------------------------------------\n");
				fprintf(stderr, "name: %s\n", _cl.client_name);
				fprintf(stderr, "flower: %s\n", _cl.flower);
				fprintf(stderr, "x: %d\n", _cl.x);
				fprintf(stderr, "y: %d\n", _cl.y);
				fprintf(stderr, "--------------------------------------------------\n");
#endif

			} else if (floristNum == 2) {

				client.distance = calcDist(florists[2].florist.x, florists[2].florist.y, client.x, client.y);
#ifdef DEBUG
				fprintf(stderr, "Insert client to queue 3\n");
#endif
				insert(floristNum, client);


#ifdef DEBUG
				struct Client _cl;
				_cl = peek(floristNum);
				fprintf(stderr, "--------------------------------------------------\n");
				fprintf(stderr, "name: %s\n", _cl.client_name);
				fprintf(stderr, "flower: %s\n", _cl.flower);
				fprintf(stderr, "x: %d\n", _cl.x);
				fprintf(stderr, "y: %d\n", _cl.y);
				fprintf(stderr, "--------------------------------------------------\n");
#endif


			} else if (floristNum == -1) {
				fprintf(stderr, "Nearest florist couldn't find!\n");
			}

		}
	}


	int count1 = countQueue1;
	int count2 = countQueue2;
	int count3 = countQueue3;

	for (int i = 0; i < count1; i++) {
		int status = pthread_create(&florist_th1[i], NULL, ThreadFlorist1, (void *) &i);
		if (status != 0){
			fprintf(stderr, "Failed to create thread number:%d : %s", i,strerror(status));
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < count2; ++i) {
		int status = pthread_create(&florist_th2[i], NULL, ThreadFlorist2, (void *) &i);
		if (status != 0){
			fprintf(stderr, "Failed to create thread number:%d : %s", i,strerror(status));
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < count3; ++i) {
		int status = pthread_create(&florist_th3[i], NULL, ThreadFlorist3, (void *) &i);
		if (status != 0){
			fprintf(stderr, "Failed to create thread number:%d : %s", i,strerror(status));
			exit(EXIT_FAILURE);
		}
	}


	for (int k = 0; k < countQueue1; ++k) {
		pthread_cond_signal(&condVarTh1);
	}
	for (int l = 0; l < countQueue2; ++l) {
		pthread_cond_signal(&condVarTh2);
	}

	for (int m = 0; m < countQueue3; ++m) {
		pthread_cond_signal(&condVarTh3);
	}


	void *value;

	int status;
	for (int j = 0; j < count1; ++j) {
		status = pthread_join(florist_th1[j], &value);
		if (status != 0) {
			fprintf(stderr, "Failed to join thread: %s\n", strerror(status));
			continue;
		}

		salesStatistics.florist1TotalTime += *(int *)value;

	}

	for (int j = 0; j < count2; ++j) {
		status = pthread_join(florist_th2[j], &value);
		if (status != 0) {
			fprintf(stderr, "Failed to join thread: %s\n", strerror(status));
			continue;
		}

		salesStatistics.florist2TotalTime += *(int *)value;

	}

	for (int j = 0; j < count3; ++j) {
		status = pthread_join(florist_th3[j], &value);
		if (status != 0) {
			fprintf(stderr, "Failed to join thread: %s\n", strerror(status));
			continue;
		}

		salesStatistics.florist3TotalTime += *(int *)value;
	}


	fprintf(stdout, RED "\nAll requests processed.\n" RESET);
	fprintf(stdout, "%s closing shop.\n", florists[0].florist.florist_name);
	fprintf(stdout, "%s closing shop.\n", florists[1].florist.florist_name);
	fprintf(stdout, "%s closing shop.\n", florists[2].florist.florist_name);


	fprintf(stdout, GRN"\nSale statistics for today:\n");
	fprintf(stdout, "-------------------------------------------------\n");
	fprintf(stdout, "Florist\t\t# of sales\t\tTotal Time\n");
	fprintf(stdout, "%s\t\t%d\t\t\t%dms\n", florists[0].florist.florist_name, salesStatistics.florist1Sales,
	        salesStatistics.florist1TotalTime);
	fprintf(stdout, "%s\t\t%d\t\t\t%dms\n", florists[1].florist.florist_name, salesStatistics.florist2Sales,
	        salesStatistics.florist2TotalTime);
	fprintf(stdout, "%s\t\t%d\t\t\t%dms\n", florists[2].florist.florist_name, salesStatistics.florist3Sales,
	        salesStatistics.florist3TotalTime);
	fprintf(stdout, "-------------------------------------------------\n" RESET);


	fclose(pInputFile);
	free(line);
	free(threadQueue1);
	free(threadQueue2);
	free(threadQueue3);
	free(florist_th1);
	free(florist_th2);
	free(florist_th3);
	free(INPUTFILENAME);
	return 0;
}

double calcDist(int floristX, int floristY, int clientX, int clientY) {
	double dx = (double) (floristX - clientX);
	double dy = (double) (floristY - clientY);

	return sqrt((dx * dx) + (dy * dy));
}


int calcEuclideanDist(struct Client client) {
	double dist = INT_MAX;
	double temp = 0.0;

#ifdef DEBUG
	fprintf(stderr, "================================================\n");
	fprintf(stderr, "client x: %d\n", client.x);
	fprintf(stderr, "client y: %d\n", client.y);
#endif

	int floristNum = -1;
	for (int i = 0; i < 3; ++i) {
#ifdef DEBUG
		fprintf(stderr, "-----------------------------------------------\n");
		fprintf(stderr, "florist x: %d\n",florists[i].florist.x);
		fprintf(stderr, "florist y: %d\n",florists[i].florist.y);
#endif
		if ((strcmp(client.flower, florists[i].florist.flower1) == 0) ||
		    (strcmp(client.flower, florists[i].florist.flower2) == 0) ||
		    (strcmp(client.flower, florists[i].florist.flower3) == 0)) {

			double dx = (double) (florists[i].florist.x - client.x);
			double dy = (double) (florists[i].florist.y - client.y);
			temp = sqrt((dx * dx) + (dy * dy));

#ifdef DEBUG
			fprintf(stderr, "\n********temp: %f\n", temp);
			fprintf(stderr, "-----------------------------------------------\n");
#endif
			if (temp < dist) {
				floristNum = i;
				dist = temp;
			}
		}

	}

#ifdef DEBUG
	fprintf(stderr, "\n********dist: %f\n", dist);
	fprintf(stderr, "\n********floristNum: %d\n", floristNum);
	fprintf(stderr, "================================================\n");
#endif
	return floristNum;

}

int parseline(char *line, struct Florist *florist, struct Client *client, int mode) {
	int parsedStatus = ERROR;
	int index = 0;
	char *buff;

	if (mode == 1) {
		parsedStatus = FLOWER;
		buff = strtok(line, " :,;\n");
		while (buff != NULL) {
			if (index == 0) {
				strcpy(florist->florist_name, buff);
			} else if (index == 1) {
				memmove(&buff[0], &buff[1], strlen(buff));
				florist->x = (int) strtol(buff, NULL, 10);
			} else if (index == 2) {
				florist->y = (int) strtol(buff, NULL, 10);
			} else if (index == 3) {
				florist->speed = strtof(buff, NULL);
			} else if (index == 4) {
				strcpy(florist->flower1, buff);
			} else if (index == 5) {
				strcpy(florist->flower2, buff);
			} else if (index == 6) {
				strcpy(florist->flower3, buff);
			}

			buff = strtok(NULL, " :,;\n");

			++index;
		}


	} else if (mode == 2) {
		parsedStatus = CLIENT;
		buff = strtok(line, " :,\n");
		while (buff != NULL) {
			if (index == 0) {
				strcpy(client->client_name, buff);
			} else if (index == 1) {
				memmove(&buff[0], &buff[1], strlen(buff));
				client->x = (int) strtol(buff, NULL, 10);
			} else if (index == 2) {
				memmove(&buff[strlen(buff - 1)], &buff[strlen(buff)], 0);
				client->y = (int) strtol(buff, NULL, 10);
			} else if (index == 3) {
				strcpy(client->flower, buff);
			}
			++index;
			buff = strtok(NULL, " :,\n");
		}

	}

	return parsedStatus;
}

void *ThreadFlorist1(void *clientNum) {
	int *floristNum = (int *) clientNum;
	struct Client _client;
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);

#ifdef DEBUG
	fprintf(stderr, "1 - Thread %d\n", *floristNum);
	fprintf(stderr, RED "isEpty: %d, count:%d\n" RESET, isEmpty(0), countQueue1);
#endif

	srand((time_t) ts.tv_nsec);

	while (isEmpty(0)) {
		pthread_cond_wait(&condVarTh1, &condVarLocTh1);
	}


	int rand_val = rand() % 50;

	_client = peek(0);
	removeData(0);
	int *delta = calloc(1, sizeof(int));
	*delta = (int) (_client.distance * rand_val);
	*delta = (int) (*delta / florists[0].florist.speed);

	usleep(*delta * 1000);

	fprintf(stdout, CYN "Florist %s has delivered a %s to %s in %dms\n" RESET, florists[0].florist.florist_name,
	        _client.flower, _client.client_name, *delta);

#ifdef DEBUG
	fprintf(stderr, "florist name: %s\n", florists[1].florist.florist_name);
	fprintf(stderr, "name: %s\n", _client.client_name);
	fprintf(stderr, "x: %d\n", _client.x);
	fprintf(stderr, "y: %d\n", _client.y);
	fprintf(stderr, "flower: %s\n", _client.flower);
#endif
	salesStatistics.florist1Sales += 1;
//	salesStatistics.florist1TotalTime += delta;
	pthread_mutex_unlock(&condVarLocTh1);

//	pthread_exit(&delta);
	return (void *)delta;
}

void *ThreadFlorist2(void *clientNum) {
	int *floristNum = (int *) clientNum;
	struct Client _client;
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);

#ifdef DEBUG
	fprintf(stderr, "2 - Thread %d\n", *floristNum);
	fprintf(stderr, BLU "isEpty: %d, count: %d\n" RESET, isEmpty(0), countQueue2);
#endif

	srand((time_t) ts.tv_nsec);
	while (isEmpty(1)) {
		pthread_cond_wait(&condVarTh2, &condVarLocTh2);
	}

	int rand_val = rand() % 50;

	_client = peek(1);
	removeData(1);
	int *delta = calloc(1, sizeof(int));
	*delta = (int) (_client.distance * rand_val);
	*delta = (int) (*delta / florists[1].florist.speed);

	usleep(*delta * 1000);

	fprintf(stdout, BLU"Florist %s has delivered a %s to %s in %dms\n" RESET, florists[1].florist.florist_name,
	        _client.flower, _client.client_name, *delta);

#ifdef DEBUG
	fprintf(stderr, "florist name: %s\n", florists[1].florist.florist_name);
	fprintf(stderr, "name: %s\n", _client.client_name);
	fprintf(stderr, "x: %d\n", _client.x);
	fprintf(stderr, "y: %d\n", _client.y);
	fprintf(stderr, "flower: %s\n", _client.flower);
#endif

	salesStatistics.florist2Sales += 1;
//	salesStatistics.florist2TotalTime += delta;
	pthread_mutex_unlock(&condVarLocTh2);

//	pthread_exit(&delta);
	return (void *)delta;
}

void *ThreadFlorist3(void *clientNum) {
	int *floristNum = (int *) clientNum;
	struct Client _client;
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
#ifdef DEBUG
	fprintf(stderr, "3 - Thread %d\n", *floristNum);
	fprintf(stderr, GRN "isEpty: %d, count: %d\n" RESET, isEmpty(0), countQueue3);
#endif

	srand((time_t) ts.tv_nsec);

	while (isEmpty(2)) {
		pthread_cond_wait(&condVarTh3, &condVarLocTh3);
	}

	int rand_val = rand() % 50;

	_client = peek(2);
	removeData(2);
	int *delta = calloc(1, sizeof(int));
	*delta = (int) (_client.distance * rand_val);
	*delta = (int) (*delta / florists[2].florist.speed);

	usleep(*delta * 1000);

	fprintf(stdout, MAG "Florist %s has delivered a %s to %s in %dms\n" RESET, florists[2].florist.florist_name,
	        _client.flower, _client.client_name, *delta);

#ifdef DEBUG
	fprintf(stderr, "florist name: %s\n", florists[1].florist.florist_name);
	fprintf(stderr, "name: %s\n", _client.client_name);
	fprintf(stderr, "x: %d\n", _client.x);
	fprintf(stderr, "y: %d\n", _client.y);
	fprintf(stderr, "flower: %s\n", _client.flower);
#endif

	salesStatistics.florist3Sales += 1;
//	salesStatistics.florist3TotalTime += delta;
	pthread_mutex_unlock(&condVarLocTh3);

//	pthread_exit(&delta);
	return (void *)delta;
}

struct Client peek(int floristNum) {
	struct Client data;
	data.y = 0;
	data.x = 0;
	strcpy(data.client_name, "none");
	strcpy(data.flower, "none");

	if (floristNum == 0) {
		data.x = threadQueue1[front1].client.x;
		data.y = threadQueue1[front1].client.y;
		strcpy(data.client_name, threadQueue1[front1].client.client_name);
		strcpy(data.flower, threadQueue1[front1].client.flower);
		data.distance = threadQueue1[front1].client.distance;
	} else if (floristNum == 1) {
		data.x = threadQueue2[front2].client.x;
		data.y = threadQueue2[front2].client.y;
		strcpy(data.client_name, threadQueue2[front2].client.client_name);
		strcpy(data.flower, threadQueue2[front2].client.flower);
		data.distance = threadQueue1[front2].client.distance;
	} else if (floristNum == 2) {
		data.x = threadQueue3[front3].client.x;
		data.y = threadQueue3[front3].client.y;
		strcpy(data.client_name, threadQueue3[front3].client.client_name);
		strcpy(data.flower, threadQueue3[front3].client.flower);
		data.distance = threadQueue1[front3].client.distance;
	}

#ifdef DEBUG
	fprintf(stderr, "data.x: %d\n", data.x);
	fprintf(stderr, "data.y: %d\n", data.y);
	fprintf(stderr, "data.flower: %s\n", data.flower);
	fprintf(stderr, "data.client_name: %s\n", data.client_name);
//	fprintf(stderr, "data.flower: %d", data.distance);
#endif

	return data;
}

bool isEmpty(int floristNum) {
	if (floristNum == 0) {
		return countQueue1 == 0;
	} else if (floristNum == 1) {
		return countQueue2 == 0;
	} else if (floristNum == 2) {
		return countQueue3 == 0;
	}

	return false;
}

bool isFull(int floristNum) {
	if (floristNum == 0) {
		return countQueue1 == maxQueueNum;
	} else if (floristNum == 1) {
		return countQueue2 == maxQueueNum;
	} else if (floristNum == 2) {
		return countQueue3 == maxQueueNum;
	}

	return false;
}

int size(int floristNum) {
	if (floristNum == 0) {
		return countQueue1;
	} else if (floristNum == 1) {
		return countQueue2;
	} else if (floristNum == 2) {
		return countQueue3;
	}
	return -1;
}

void insert(int floristNum, struct Client newClient) {
	if (floristNum == 0) {
		if (!isFull(floristNum)) {

			if (rear1 == maxQueueNum - 1) {
				rear1 = -1;
			}
			++rear1;
			strcpy(threadQueue1[rear1].client.client_name, newClient.client_name);
			strcpy(threadQueue1[rear1].client.flower, newClient.flower);
			threadQueue1[rear1].client.x = newClient.x;
			threadQueue1[rear1].client.y = newClient.y;
			threadQueue1[rear1].client.distance = newClient.distance;
#ifdef DEBUG
			fprintf(stderr, "threadQueue1[%d].client.x: %d\n", rear1, threadQueue1[rear1].client.x);
			fprintf(stderr, "threadQueue1[%d].client.y: %d\n", rear1, threadQueue1[rear1].client.y);
			fprintf(stderr, "threadQueue1[%d].client.flower: %s\n", rear1, threadQueue1[rear1].client.flower);
			fprintf(stderr, "threadQueue1[%d].client.client_name: %s\n", rear1, threadQueue1[rear1].client.flower);
#endif
			countQueue1++;
		}
	} else if (floristNum == 1) {
		if (!isFull(floristNum)) {

			if (rear2 == maxQueueNum - 1) {
				rear2 = -1;
			}
			++rear2;
//			threadQueue2[++rear].client = newClient;
			strcpy(threadQueue2[rear2].client.client_name, newClient.client_name);
			strcpy(threadQueue2[rear2].client.flower, newClient.flower);
			threadQueue2[rear2].client.x = newClient.x;
			threadQueue2[rear2].client.y = newClient.y;
			threadQueue2[rear2].client.distance = newClient.distance;
#ifdef DEBUG
			fprintf(stderr, "threadQueue2[%d].client.x: %d\n", rear2, threadQueue2[rear2].client.x);
			fprintf(stderr, "threadQueue2[%d].client.y: %d\n", rear2, threadQueue2[rear2].client.y);
			fprintf(stderr, "threadQueue2[%d].client.flower: %s\n", rear2, threadQueue2[rear2].client.flower);
			fprintf(stderr, "threadQueue2[%d].client.client_name: %s\n", rear2, threadQueue2[rear2].client.flower);
#endif
			countQueue2++;
		}
	} else if (floristNum == 2) {
		if (!isFull(floristNum)) {

			if (rear3 == maxQueueNum - 1) {
				rear3 = -1;
			}
			++rear3;
//			threadQueue3[++rear].client = newClient;
			strcpy(threadQueue3[rear3].client.client_name, newClient.client_name);
			strcpy(threadQueue3[rear3].client.flower, newClient.flower);
			threadQueue3[rear3].client.x = newClient.x;
			threadQueue3[rear3].client.y = newClient.y;
			threadQueue3[rear3].client.distance = newClient.distance;
#ifdef DEBUG
			fprintf(stderr, "threadQueue3[%d].client.x: %d\n", rear3, threadQueue3[rear3].client.x);
			fprintf(stderr, "threadQueue3[%d].client.y: %d\n", rear3, threadQueue3[rear3].client.y);
			fprintf(stderr, "threadQueue3[%d].client.flower: %s\n", rear3, threadQueue3[rear3].client.flower);
			fprintf(stderr, "threadQueue3[%d].client.client_name: %s\n", rear3, threadQueue3[rear3].client.flower);
#endif
			countQueue3++;
		}
	}

}

struct Client removeData(int floristNum) {
	struct Client data;
	strcpy(data.flower, "none");
	strcpy(data.client_name, "none");
	data.x = 0;
	data.y = 0;

	if (floristNum == 0) {

		strcpy(data.flower, threadQueue1[front1].client.flower);
		strcpy(data.client_name, threadQueue1[front1].client.client_name);
		data.x = threadQueue1[front1].client.x;
		data.y = threadQueue1[front1].client.y;

		front1++;
		if (front1 == maxQueueNum) {
			front1 = 0;
		}

		countQueue1--;

	} else if (floristNum == 1) {

		strcpy(data.flower, threadQueue2[front2].client.flower);
		strcpy(data.client_name, threadQueue2[front2].client.client_name);
		data.x = threadQueue2[front2].client.x;
		data.y = threadQueue2[front2].client.y;


		front2++;
		if (front2 == maxQueueNum) {
			front2 = 0;
		}

		countQueue2--;

	} else if (floristNum == 2) {
		strcpy(data.flower, threadQueue3[front3].client.flower);
		strcpy(data.client_name, threadQueue3[front3].client.client_name);
		data.x = threadQueue3[front3].client.x;
		data.y = threadQueue3[front3].client.y;

		front3++;

		if (front3 == maxQueueNum) {
			front3 = 0;
		}

		countQueue3--;

	}
	return data;
}