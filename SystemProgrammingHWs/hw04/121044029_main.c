#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <semaphore.h>

#define SHM_NAME 						"SHMCHEFWHOLESALER"
#define FLAGS 							O_RDWR | O_CREAT | O_EXCL
#define FLAGS_SEM 					O_CREAT | O_EXCL
#define PERMS 							S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP
#define SHM_SIZE 						1024
#define SEM_WHOLESALER 			"SEMWHOLESALER"
#define SEM_CHEF1 					"SEMCHEF1"
#define SEM_CHEF2 					"SEMCHEF2"
#define SEM_CHEF3 					"SEMCHEF3"
#define SEM_CHEF4 					"SEMCHEF4"
#define SEM_CHEF5 					"SEMCHEF5"
#define SEM_CHEF6 					"SEMCHEF6"
#define SEM_DESERTREADY 		"SENDESERTREADY"

const int chef_num = 5;
int shmID;

struct Ingredients {
		bool Eggs;
		bool Flour;
		bool Butter;
		bool Sugar;
};

volatile static bool is_running = true;
static sem_t	*wholesaler_ready;
static sem_t *chef_sem1;
static sem_t *chef_sem2;
static sem_t *chef_sem3;
static sem_t *chef_sem4;
static sem_t *chef_sem5;
static sem_t *chef_sem6;
static sem_t *desert_ready;

// prepared by prepare_ingredients_types func
struct Ingredients ingredients_types[6];
struct Ingredients *ing_sh_mem;
void chef(int chef_num);
void whole_saler();
void prepare_ingredients_types();
void initialize();
void signal_catcher(int signum);

int main(int argc, char const *argv[]) {
		pid_t child_pid;
		struct sigaction sact;
		sigset_t block_mask, orig_mask;

		sigemptyset(&block_mask);
		sigaddset(&block_mask, SIGINT);
		sact.sa_flags = 0;
		sact.sa_handler = &signal_catcher;

		if (sigaction(SIGINT, &sact, NULL) != 0) {
				perror("SIGINT sigaction error");
				exit(EXIT_FAILURE);
		}
		sem_unlink(SEM_WHOLESALER);
		sem_unlink(SEM_CHEF1);
		sem_unlink(SEM_CHEF2);
		sem_unlink(SEM_CHEF3);
		sem_unlink(SEM_CHEF4);
		sem_unlink(SEM_CHEF5);
		sem_unlink(SEM_CHEF6);
		sem_unlink(SEM_DESERTREADY);
		shm_unlink(SHM_NAME);

		initialize();
		prepare_ingredients_types();

		int i = 0;
		for (i = 0; i <= chef_num; i++) {
				if ((child_pid = fork()) <= 0) {
						break;
				}
		}

		if (child_pid == -1) {
				fprintf(stderr, "Failed fork3: %s\n", strerror(errno));
		} else if (child_pid == 0) { // child process
				chef(i);
		} else { // parent process
				whole_saler();

				while (wait(NULL) != -1) {}
		}

		fprintf(stdout, "Exiting the program\n");
		sem_close(wholesaler_ready);
		sem_close(chef_sem1);
		sem_close(chef_sem2);
		sem_close(chef_sem3);
		sem_close(chef_sem4);
		sem_close(chef_sem5);
		sem_close(chef_sem6);
		sem_close(desert_ready);

		sem_unlink(SEM_WHOLESALER);
		sem_unlink(SEM_CHEF1);
		sem_unlink(SEM_CHEF2);
		sem_unlink(SEM_CHEF3);
		sem_unlink(SEM_CHEF4);
		sem_unlink(SEM_CHEF5);
		sem_unlink(SEM_CHEF6);
		sem_unlink(SEM_DESERTREADY);
		shm_unlink(SHM_NAME);

		return 0;
}
void signal_catcher(int signum) {
	switch (signum) {
		case SIGINT:
			fprintf(stderr, "Catched SIGINT signal\n");
			is_running = false;
			break;
	}
}


void whole_saler() {
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		int rand_val = 0;
		bool first_run = true;
		struct Ingredients false_ing;

		srand((time_t)ts.tv_nsec);

		while(is_running) {
			fprintf(stderr, "===================================================\n");
			// wait for a lock
			if (sem_wait(wholesaler_ready) == -1){
				fprintf(stderr, "sem_wait failed: %s\n",strerror(errno));
				// TODO: burada kill atip cocuklarin kapanmasini saglaman gerekebilir.
			}

			rand_val = rand() % chef_num;

			memcpy(ing_sh_mem, &ingredients_types[rand_val], sizeof(ingredients_types[rand_val]));

			if (ingredients_types[rand_val].Eggs && ingredients_types[rand_val].Butter) {
					fprintf(stderr, "wholesaler delivers eggs and butter.... \n");
			} else if(ingredients_types[rand_val].Eggs && ingredients_types[rand_val].Sugar) {
					fprintf(stderr, "wholesaler delivers eggs and sugar... \n");
			} else if (ingredients_types[rand_val].Eggs && ingredients_types[rand_val].Flour) {
					fprintf(stderr, "wholesaler delivers eggs and flour... \n");
			} else if (ingredients_types[rand_val].Butter && ingredients_types[rand_val].Flour) {
					fprintf(stderr, "wholesaler delivers butter and flour... \n");
			} else if (ingredients_types[rand_val].Butter && ingredients_types[rand_val].Sugar) {
					fprintf(stderr, "wholesaler delivers butter and sugar... \n");
			} else if (ingredients_types[rand_val].Sugar && ingredients_types[rand_val].Flour) {
					fprintf(stderr, "wholesaler delivers sugar and flour... \n");
			}


			if(rand_val == 0){
				if (sem_post(chef_sem1) == -1){
						fprintf(stderr, "Chef1 semaphore: sem_post failed: %s\n", strerror(errno));
						_exit(EXIT_FAILURE);
				}
			} else if(rand_val == 1){
				if (sem_post(chef_sem2) == -1){
					fprintf(stderr, "Chef2 semaphore: sem_post failed: %s\n", strerror(errno));
					_exit(EXIT_FAILURE);
				}
			} else if(rand_val == 2) {
				if (sem_post(chef_sem3) == -1){
					fprintf(stderr, "Chef3 semaphore: sem_post failed: %s\n", strerror(errno));
					_exit(EXIT_FAILURE);
				}
			} else if(rand_val == 3) {
				if (sem_post(chef_sem4) == -1){
					fprintf(stderr, "Chef4 semaphore: sem_post failed: %s\n", strerror(errno));
					_exit(EXIT_FAILURE);
				}
			} else if(rand_val == 4) {
				if (sem_post(chef_sem5) == -1){
					fprintf(stderr, "Chef5 semaphore: sem_post failed: %s\n", strerror(errno));
					_exit(EXIT_FAILURE);
				}
			} else if(rand_val == 5) {
				if (sem_post(chef_sem6) == -1){
					fprintf(stderr, "Chef6 semaphore: sem_post failed: %s\n", strerror(errno));
					_exit(EXIT_FAILURE);
				}
			}

			fprintf(stderr, "wholesaler waiting for the desert\n");

			if (sem_wait(desert_ready) == -1) {
					fprintf(stderr, "sem_wait failed: %s\n",strerror(errno));
					_exit(EXIT_FAILURE);
			}

			fprintf(stderr, "wholesaler has obtained the dessert and left to sell it\n");
			fprintf(stderr, "===================================================\n");
		}

		return;
}


void chef(int chef_num) {
		bool status = true;
		bool is_chef_ok = false;

		while(is_running) {

			if (chef_num == 0 && status) {
				fprintf(stderr, "Chef1 is waiting sugar and flour\n");
				status = false;
				if (sem_wait(chef_sem1) == -1) {
					fprintf(stderr, "sem_wait failed. exiting chef1: %s\n", strerror(errno));
					_exit(EXIT_FAILURE);
				}
				fprintf(stderr, "lock 1\n");
			} else if (chef_num == 1 && status) {
				fprintf(stderr, "Chef2 is waiting butter and flour\n");
				status = false;
				if (sem_wait(chef_sem2) == -1) {
					fprintf(stderr, "sem_wait failed. exiting chef2: %s\n", strerror(errno));
					_exit(EXIT_FAILURE);
				}
				fprintf(stderr, "lock 2\n");
			} else if (chef_num == 2 && status) {
				fprintf(stderr, "Chef3 is waiting butter and sugar\n");
				status = false;
				if (sem_wait(chef_sem3) == -1) {
					fprintf(stderr, "sem_wait failed. exiting chef3: %s\n", strerror(errno));
					_exit(EXIT_FAILURE);
				}
				fprintf(stderr, "lock 3\n");
			} else if (chef_num == 3 && status) {
				fprintf(stderr, "Chef4 is waiting sugar and eggs\n");
				status = false;
				if (sem_wait(chef_sem4) == -1) {
					fprintf(stderr, "sem_wait failed. exiting chef4: %s\n", strerror(errno));
					_exit(EXIT_FAILURE);
				}
				fprintf(stderr, "lock 4\n");
			} else if (chef_num == 4 && status) {
				fprintf(stderr, "Chef5 is waiting flour and eggs\n");
				status = false;
				if (sem_wait(chef_sem5) == -1) {
					fprintf(stderr, "sem_wait failed. exiting chef6: %s\n", strerror(errno));
					_exit(EXIT_FAILURE);
				}
				fprintf(stderr, "lock 5\n");
			} else if (chef_num == 5 && status) {
				fprintf(stderr, "Chef6 is waiting butter and eggs\n");
				status = false;
				if (sem_wait(chef_sem6) == -1) {
					fprintf(stderr, "sem_wait failed. exiting chef6: %s\n", strerror(errno));
					_exit(EXIT_FAILURE);
				}
				fprintf(stderr, "lock 6\n");
			}

			if (ing_sh_mem->Eggs && ing_sh_mem->Butter && chef_num == 5) { // buraya chef6 girmeli
					fprintf(stderr, "Chef%d has taken eggs and butter\n",chef_num+1);
					if (sem_post(wholesaler_ready) == -1) {
						fprintf(stderr, "sem_post failed. exiting %d chef: %s\n",chef_num+1, strerror(errno));
						_exit(EXIT_FAILURE);
					}
					is_chef_ok = true;
			} else if (ing_sh_mem->Eggs && ing_sh_mem->Flour && chef_num == 4) { // buarya chef5 girmeli
					fprintf(stderr, "Chef%d has taken eggs and flour\n", chef_num+1);
					if (sem_post(wholesaler_ready) == -1) {
						fprintf(stderr, "sem_post failed. exiting %d chef: %s\n",chef_num+1, strerror(errno));
						_exit(EXIT_FAILURE);
					}
					is_chef_ok = true;
			} else if (ing_sh_mem->Eggs && ing_sh_mem->Sugar && chef_num == 3) { // buraya chef4 girmeli
					fprintf(stderr, "Chef%d has taken eggs and sugar\n", chef_num+1);
					if (sem_post(wholesaler_ready) == -1) {
						fprintf(stderr, "sem_post failed. exiting %d chef: %s\n",chef_num+1, strerror(errno));
						_exit(EXIT_FAILURE);
					}
					is_chef_ok = true;
			} else if (ing_sh_mem->Butter && ing_sh_mem->Sugar && chef_num == 2) { //buraya chef3 girmeli
					fprintf(stderr, "Chef%d has taken butter and sugar\n", chef_num+1);
					if (sem_post(wholesaler_ready) == -1) {
						fprintf(stderr, "sem_post failed. exiting %d chef: %s\n",chef_num+1, strerror(errno));
						_exit(EXIT_FAILURE);
					}
					is_chef_ok = true;
			} else if (ing_sh_mem->Butter && ing_sh_mem->Flour && chef_num == 1) { // buraya chef2 girmeli
					fprintf(stderr, "Chef%d has taken butter and flour\n", chef_num+1);
					if (sem_post(wholesaler_ready) == -1) {
						 fprintf(stderr, "sem_post failed. exiting %d chef: %s\n",chef_num+1, strerror(errno));
						_exit(EXIT_FAILURE);
					}
					is_chef_ok = true;
			} else if (ing_sh_mem->Sugar && ing_sh_mem->Flour && chef_num == 0) { // buraya chef1 girmeli
					fprintf(stderr, "Chef%d has taken sugar and flour\n", chef_num+1);
					if (sem_post(wholesaler_ready) == -1) {
						fprintf(stderr, "sem_post failed. exiting %d chef: %s\n",chef_num+1, strerror(errno));
						_exit(EXIT_FAILURE);
					}
					is_chef_ok = true;
			}

			if (is_chef_ok) {
					is_chef_ok = false;
					fprintf(stderr, "Chef%d is preparing the dessert\n", chef_num+1);
					fprintf(stderr, "Chef%d has delivered the dessert to the wholesaler\n",chef_num+1);
					status = true;

					if (sem_post(desert_ready) == -1){
						fprintf(stderr, "sem_post failed. exiting chef%d: %s\n",chef_num+1, strerror(errno) );
						_exit(EXIT_FAILURE);
					}
			}

		}

		_exit(EXIT_SUCCESS);
}

void prepare_ingredients_types() {
		ingredients_types[0].Eggs = false;
		ingredients_types[0].Flour = true;
		ingredients_types[0].Butter = false;
		ingredients_types[0].Sugar = true;

		ingredients_types[1].Eggs = false;
		ingredients_types[1].Flour = true;
		ingredients_types[1].Butter = true;
		ingredients_types[1].Sugar = false;

		ingredients_types[2].Eggs = false;
		ingredients_types[2].Flour = false;
		ingredients_types[2].Butter = true;
		ingredients_types[2].Sugar = true;

		ingredients_types[3].Eggs = true;
		ingredients_types[3].Flour = false;
		ingredients_types[3].Butter = false;
		ingredients_types[3].Sugar = true;

		ingredients_types[4].Eggs = true;
		ingredients_types[4].Flour = true;
		ingredients_types[4].Butter = false;
		ingredients_types[4].Sugar = false;

		ingredients_types[5].Eggs = true;
		ingredients_types[5].Flour = false;
		ingredients_types[5].Butter = true;
		ingredients_types[5].Sugar = false;
}

void initialize() {
		shmID = shm_open(SHM_NAME, FLAGS, PERMS);
		if (shmID == -1) {
				if (strcmp(strerror(errno), "File exists") == 0) {
						if (shm_unlink(SHM_NAME) == -1){
							fprintf(stderr, "Failed to remove shared memory file: %s\n", strerror(errno));
							exit(EXIT_FAILURE);
						}
						fprintf(stderr, "Unlink already exist shared memory\n");
						shmID = shm_open(SHM_NAME, FLAGS, PERMS);
				}else {
						fprintf(stderr, "Failed to open sherad memory: %s\n", strerror(errno) );
						exit(EXIT_FAILURE);
				}
		}

		if (ftruncate(shmID, sizeof(struct Ingredients)) == -1) {
				fprintf(stderr, "Failed sizing sherad memory %s\n",strerror(errno) );
				exit(EXIT_FAILURE);
		}

		ing_sh_mem = (struct Ingredients *)mmap(NULL, sizeof(struct Ingredients), PROT_READ | PROT_WRITE, MAP_SHARED, shmID, 0);
		if (ing_sh_mem == MAP_FAILED) {
				fprintf(stderr, "MMAP function failed: %s\n",strerror(errno));
				exit(EXIT_FAILURE);
		}

		wholesaler_ready = sem_open(SEM_WHOLESALER, FLAGS_SEM, PERMS, 1);
		if (wholesaler_ready == SEM_FAILED) {
				fprintf(stderr, "Wholesaler semaphore open failed: %s\n",strerror(errno));
				exit(EXIT_FAILURE);
		}

		chef_sem1 = sem_open(SEM_CHEF1, FLAGS_SEM, PERMS, 0);
		if (chef_sem1 == SEM_FAILED) {
				fprintf(stderr, "Chef1 semaphore open failed: %s\n",strerror(errno));
				exit(EXIT_FAILURE);
		}

		chef_sem2 = sem_open(SEM_CHEF2, FLAGS_SEM, PERMS, 0);
		if (chef_sem2 == SEM_FAILED) {
				fprintf(stderr, "Chef2 semaphore open failed: %s\n",strerror(errno));
				exit(EXIT_FAILURE);
		}

		chef_sem3 = sem_open(SEM_CHEF3, FLAGS_SEM, PERMS, 0);
		if (chef_sem3 == SEM_FAILED) {
				fprintf(stderr, "Chef3 semaphore open failed: %s\n",strerror(errno));
				exit(EXIT_FAILURE);
		}

		chef_sem4 = sem_open(SEM_CHEF4, FLAGS_SEM, PERMS, 0);
		if (chef_sem4 == SEM_FAILED) {
				fprintf(stderr, "Chef4 semaphore open failed: %s\n",strerror(errno));
				exit(EXIT_FAILURE);
		}

		chef_sem5 = sem_open(SEM_CHEF5, FLAGS_SEM, PERMS, 0);
		if (chef_sem5 == SEM_FAILED) {
				fprintf(stderr, "Chef5 semaphore open failed: %s\n",strerror(errno));
				exit(EXIT_FAILURE);
		}

		chef_sem6 = sem_open(SEM_CHEF6, FLAGS_SEM, PERMS, 0);
		if (chef_sem6 == SEM_FAILED) {
				fprintf(stderr, "Chef6 semaphore open failed: %s\n",strerror(errno));
				exit(EXIT_FAILURE);
		}

		desert_ready = sem_open(SEM_DESERTREADY, FLAGS_SEM, PERMS, 0);
		if (desert_ready == SEM_FAILED) {
				fprintf(stderr, "Desert ready semaphore open failed: %s\n",strerror(errno));
				exit(EXIT_FAILURE);
		}

}
