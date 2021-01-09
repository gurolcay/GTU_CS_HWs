//
// Created by Gurol CAY on 2.04.2018.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/file.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define PI 3.14
#define PI2 6.2832
#define PROCESS_A_LOG_FILE_NAME "processA.log"
#define PROCESS_B_LOG_FILE_NAME "processB.log"


struct RealNumber {
		double r_number;
		double i_number;
};

int is_running = 1;
int N = 0, M = 0;
FILE *p_log_file_process_a;
FILE *p_log_file_process_b;
char *file_name;

void start_process_A(pid_t child_pid, sigset_t orig_mask, int file_desc);
int start_process_B(sigset_t orig_mask, int file_desc);
void signal_catcher(int signum);
void produce_random_real_numbers(double *real_numbers);
void DFT(double* numbers, struct RealNumber *transformed_result);
void print_produced_numbers(double *numbers, int line);
void print_process_b_infos(double *numbers, int line);

int main(int argc, char const *argv[]) {
		pid_t child_pid;
		pid_t wait_pid;
		sigset_t block_mask, orig_mask;
		struct sigaction sact;
		int fd;

		memset(&sact, 0, sizeof(sact));

		sigemptyset(&block_mask);
		sigaddset(&block_mask, SIGUSR1);
		sigaddset(&block_mask, SIGUSR2);
		sigaddset(&block_mask, SIGINT);
		sact.sa_flags = 0;
		sact.sa_handler = &signal_catcher;


		if (argc != 7) {
				fprintf(stdout, "Usage: ./%s -N <integer> -X <file.dat> -M <integer>\n", argv[0] );
				exit(EXIT_FAILURE);
		}
		if (!((strcmp(argv[1], "-N") == 0) && (strcmp(argv[3], "-X") == 0) && (strcmp(argv[5], "-M") == 0))) {
				fprintf(stdout, "Usage: ./%s -N <integer> -X <file.dat> -M <integer>\n", argv[0] );
				exit(EXIT_FAILURE);
		}

		N = (int)strtol(argv[2], NULL, 10);

		if (errno == EINVAL) {
				perror("N value is not integer");
				exit(EXIT_FAILURE);
		}

		M = (int)strtol(argv[6], NULL, 10);

		if (errno == EINVAL) {
				perror("M value is not integer");
				exit(EXIT_FAILURE);
		}
		file_name = calloc(strlen(argv[4]), sizeof(char));
		strcpy(file_name, argv[4]);

		fd = open(argv[4], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

		if (fd == -1) {
				fprintf(stderr, "Failed to open: %s : %s\n",argv[4], strerror(errno) );
				exit(EXIT_FAILURE);
		}

		if (sigaction(SIGUSR2, &sact, NULL) != 0) {
				perror("Parent sigaction error");
				exit(EXIT_FAILURE);
		}

		if (sigaction(SIGUSR1, &sact, NULL) != 0) {
				perror("Child sigaction error");
				exit(EXIT_FAILURE);
		}

		if (sigaction(SIGINT, &sact, NULL) != 0) {
				perror("SIGINT sigaction error");
				exit(EXIT_FAILURE);
		}

		if (sigprocmask(SIG_BLOCK, &block_mask, &orig_mask) == -1) {
				perror("sigprocmask - SIG_BLOCK");
				exit(EXIT_FAILURE);
		}


		child_pid = fork();
		if (child_pid == -1) {
				fprintf(stderr, "Fork failed: %s\n", strerror(errno));
				exit(EXIT_FAILURE);
		}

		if (child_pid == 0) {   // child code
				if (start_process_B(orig_mask, fd)) {
						_exit(EXIT_SUCCESS);
				} else {
						_exit(EXIT_FAILURE);
				}
		}
		// parent code
		start_process_A(child_pid, orig_mask, fd);

		wait_pid = wait(NULL);
		if (wait_pid == -1) {
				if (errno == ECHILD) {
						fprintf(stderr, "No more children.\n");
						exit(EXIT_SUCCESS);
				} else {
						exit(EXIT_FAILURE);
				}
		}
		free(file_name);
		close(fd);
		return 0;
}

void signal_catcher(int signum) {
		switch (signum) {
		// case SIGUSR1:
		//      fprintf(stderr, "Parent catch signal \n");
		//      break;
		// case SIGUSR2:
		//      fprintf(stderr, "Child catch signal \n");
		//      break;
		case SIGINT:
				is_running = 0;
				fprintf(stderr, "Process A catch signal, it will be done.\n");
				fprintf(stderr, "Process B catch signal, it will be done.\n");
				if (p_log_file_process_a)
						fprintf(p_log_file_process_a, "\nProcess A catch signal, it will be done.\n");
				if (p_log_file_process_b)
						fprintf(p_log_file_process_b, "\nProcess B catch signal, it will be done.\n");
				break;
		}
}

void produce_random_real_numbers(double *real_numbers) {
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		srand((time_t)ts.tv_nsec);

		for (int i = 0; i < N; ++i) {
				real_numbers[i] = rand() % 153 / 2.0;
		}
}

void start_process_A(pid_t child_pid, sigset_t orig_mask, int file_desc){    // parent function
		int fd = -1, line = 0;
		int write_bytes = 0;
		double *real_numbers;

		// struct stat st;
		unsigned long position = 0;

		p_log_file_process_a = fopen(PROCESS_A_LOG_FILE_NAME, "w+");
		if (!p_log_file_process_a)
				fprintf(stderr, "Failed to open log file for process A. Continuing without log file!!!!\n");

		real_numbers = (double *)calloc(N, sizeof(double));

		while (is_running) {

				produce_random_real_numbers(real_numbers);

				flock(file_desc, LOCK_SH);

				// write_bytes = write(file_desc, real_numbers, sizeof(double)*N);
				while(((write_bytes = write(file_desc, real_numbers, sizeof(double)*N)) == -1) && errno == EINTR);
				// printf("write_bytes: %d\n",write_bytes );
				position += write_bytes;
				// stat(file_name, &st);
				// fprintf(stderr, "file size: %lld\n",st.st_size );
				line = (line + 1) % N;

				print_produced_numbers(real_numbers, line);

				if (write_bytes == -1) {
						fprintf(stderr, "Failed to write file by the parent: %s\n", strerror(errno));
						if (p_log_file_process_a)
								fprintf(p_log_file_process_a, "Failed to write file by the parent: %s\n", strerror(errno));
						_exit(EXIT_FAILURE);
				}

				if (write_bytes != sizeof(double)*N) {
						fprintf(stderr, "Failed to write file by the parent: Unknown problem\n");
						if (p_log_file_process_a)
								fprintf(p_log_file_process_a, "Failed to write file by the parent: Unknown problem\n");
						_exit(EXIT_FAILURE);
				}

				write_bytes = 0;
				kill(child_pid, SIGUSR2);               // send signal to child

				flock(file_desc, LOCK_UN);
				if (sigsuspend(&orig_mask) == -1 && errno != EINTR) {
						perror("Parent sigsuspend function error");
						if (p_log_file_process_a)
								fprintf(stdout,"Parent sigsuspend function error: %s", strerror(errno));
						exit(EXIT_FAILURE);
				}
				// sleep(3);
		}
		free(real_numbers);
		fclose(p_log_file_process_a);
		_exit(EXIT_SUCCESS);
}


// TODO hangi satirda oldugunu dosya boyutunu yazdigin her bir satirin boyutuna
// TODO bolerek bulabilirsin sanirim


int start_process_B(sigset_t orig_mask, int file_desc) {    // child function
		int fd = -1, line;
		int read_bytes = 0;
		double *read_arr;
		double dbl;
		unsigned long position = 0;


		p_log_file_process_b = fopen(PROCESS_B_LOG_FILE_NAME, "w+");

		if (!p_log_file_process_b) {
				fprintf(stderr, "Failed to open log file for process B. Continuing without log file!!!!\n");
		}
		// TODO burada en fazla M sequence alabilirsin, buna dikkat etmen lazim
		read_arr = (double *)calloc(N, sizeof(double));

		while (is_running) {

				if (sigsuspend(&orig_mask) == -1 && errno != EINTR) {
						perror("Child sigsuspend function error");
						if (p_log_file_process_b) {
								fprintf(p_log_file_process_b, "Child sigsuspend function error: %s\n",strerror(errno));
						}
						exit(EXIT_FAILURE);
				}

				flock(file_desc, LOCK_SH);
				lseek(file_desc, position, SEEK_SET);
				while(((read_bytes = read(file_desc, read_arr, sizeof(double)*N)) == -1) && errno == EINTR);

				line = (line + 1) % N;
				// while ((cnt = read(fd, buf, BUF_SIZE)) == -1 && errno == EINTR)
				position += read_bytes;
				print_process_b_infos(read_arr, line);
				// fprintf(stderr, "read_bytes: %d\n",read_bytes );
				if (read_bytes == -1) {
						perror("Failed to read intermediary file");
						if (p_log_file_process_b) {
								fprintf(p_log_file_process_b, "Failed to read intermediary file: %s\n",strerror(errno));
						}
						_exit(EXIT_FAILURE);
				}

				read_bytes = 0;

				flock(file_desc, LOCK_UN);
				kill(getppid(), SIGUSR1);
				// sleep(3);

		}
		free(read_arr);
		fclose(p_log_file_process_b);
		return 1;
}

void print_process_b_infos(double *numbers, int line) {
		int i = 0;
		struct RealNumber *transformed_result;
		transformed_result = calloc(N, sizeof(struct RealNumber));

		fprintf(stdout, "Process B: the dft of line %d (",line );
		if (p_log_file_process_b)
				fprintf(p_log_file_process_b, "Process B: the dft of line %d (",line );

		for (i = 0; i < N; ++i) {
				fprintf(stdout, "%0.2f ", numbers[i]);
				if (p_log_file_process_b)
						fprintf(p_log_file_process_b, "%0.2f ", numbers[i]);
		}

		fprintf(stdout, ") is: ");
		if (p_log_file_process_b)
				fprintf(p_log_file_process_b, ") is: ");

		DFT(numbers, transformed_result);

		for (i = 0; i < N; ++i) {
				if (transformed_result[i].i_number < 0) {
						if (p_log_file_process_b)
								fprintf(p_log_file_process_b, "%0.2f%0.2fi ", transformed_result[i].r_number, transformed_result[i].i_number);
						fprintf(stdout, "%0.2f%0.2fi ", transformed_result[i].r_number, transformed_result[i].i_number);
				} else {
						if (p_log_file_process_b)
								fprintf(p_log_file_process_b, "%0.2f+%0.2fi ", transformed_result[i].r_number, transformed_result[i].i_number);
						fprintf(stdout, "%0.2f+%0.2fi ", transformed_result[i].r_number, transformed_result[i].i_number);
				}

		}
		fprintf(stdout, "\n");
		if (p_log_file_process_b)
				fprintf(p_log_file_process_b, "\n");

		free(transformed_result);
}

void print_produced_numbers(double *numbers, int line) {
		int i = 0;

		fprintf(stdout, "Process A: i'm producing a random sequence for line %d ",line);
		if (p_log_file_process_a)
				fprintf(p_log_file_process_a, "Process A: i'm producing a random sequence for line %d ",line);

		for (i = 0; i < N; ++i) {
				fprintf(stdout, "%0.2f ",numbers[i]);
				if (p_log_file_process_a)
						fprintf(p_log_file_process_a, "%0.2f ",numbers[i]);
		}
		fprintf(stdout, "\n");
		if (p_log_file_process_a)
				fprintf(p_log_file_process_a, "\n");
}

/**
 * I got some help while I developed this function.
 * https://batchloaf.wordpress.com/2013/12/07/simple-dft-in-c/
 *
 */
void DFT(double* numbers, struct RealNumber *transformed_result) {
		int i = 0, n = 0;

		for (i = 0; i < N; ++i) {
				for (n = 0; n < N; ++n)
						transformed_result[i].r_number += numbers[n] * cos(n * i * PI2 / N);

				for (n = 0; n < N; ++n)
						transformed_result[i].i_number -= numbers[n] * sin(n * i * PI2 / N);
		}
}
