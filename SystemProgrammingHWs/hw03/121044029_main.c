//
// Created by Gurol CAY on 27.04.2018.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

// Magic values
#define CMD_SIZE      256
#define MAX_STR_SIZE  2048
#define MAX_CMD_NUM   10
#define HISTORY_SIZE  100
#define DELIMETERS    " \t\n\a"

// Available commands
#define LS      "ls"
#define PWD     "pwd"
#define CD      "cd"
#define HELP    "help"
#define CAT     "cat"
#define WC      "wc"
#define EXIT    "exit"
#define PIPE    "|"
#define LARROW  "<"
#define LLARROW "<<"
#define RARROW  ">"
#define RRARROW ">>"
// End of available commands

#define UP_KEY      30
#define DOWN_KEY    40
#define GO          50

#define STDIN       0
#define STDOUT      1
#define PIPE_MODE   100   // wc program takes input from pipe
#define FILE_MODE   200   // wc program takes filename as input
#define REDIRECT_M  300   // wc program write to results to file example usage: wc filename > output.txt
#define APPEND_MODE 400
// Function decleration
void start_console();
int execute_commands(char **cmds, int cmd_num);
int parse_line(char *line, char **cmds);
int check_commands(const char **cmds, int cmd_num, int num);
int read_commands(char *line );
char *execute_ls(char *arg);
char *execute_wc(char *arg, int mode);
void execute_rredirect(char *arg, char *filename, int mode);
void signal_catcher(int signum);
void print_help();

int signal_catched = 0;

int main(int argc, char const *argv[]) {
  sigset_t block_mask, orig_mask;
  struct sigaction sact;

	memset(&sact, 0, sizeof(sact));

  sigemptyset(&block_mask);
  sigaddset(&block_mask, SIGTERM);
  sact.sa_flags = 0;
  sact.sa_handler = &signal_catcher;

  if (sigaction(SIGTERM, &sact, NULL) != 0) {
      perror("SIGINT sigaction error");
      exit(EXIT_FAILURE);
  }

  if (sigprocmask(SIG_BLOCK, &block_mask, &orig_mask) == -1) {
      perror("sigprocmask - SIG_BLOCK");
      exit(EXIT_FAILURE);
  }
  fprintf(stdout, "If you want to get help, you can type -help- command\n");
  fprintf(stdout, "If you want to exit from program, you can type -exit- command\n");
  start_console();

  return 0;
}

void signal_catcher(int signum) {
		switch (signum) {
	    case SIGTERM:
				fprintf(stderr, "\nSIGTERM signal catched\n");
        signal_catched = 1;
				break;
		}
}

void start_console(){
  char **history, **cmds;
  char line[CMD_SIZE];
  int cmds_num = 0, history_counter = 0,
      r_status = 0, status = 0, entered_cmd = 0;

  memset(line, '\0', CMD_SIZE);

  cmds = (char **)calloc(MAX_CMD_NUM, sizeof(char *));
  for (int i = 0; i < MAX_CMD_NUM; i++) {
    cmds[i] = (char *)calloc(CMD_SIZE, sizeof(char));
  }

  history = (char **)calloc(HISTORY_SIZE, sizeof(char *));
  for (int i = 0; i < HISTORY_SIZE; i++) {
    history[i] = (char *)calloc(CMD_SIZE, sizeof(char));
  }

  for (;!signal_catched;) {
    fprintf(stdout, "\nYou can type command: \n");
    r_status = read_commands(line);
    if (r_status == -1) {
      exit(EXIT_FAILURE);
    } else if (r_status == UP_KEY) {
      --history_counter;
      fprintf(stdout, "%s\n", history[history_counter]);
      // maybe this is not necessary
      // strcpy(line, history[history_counter]);
      // r_status = GO;
    } else if (r_status == DOWN_KEY) {
      ++history_counter;
      fprintf(stdout, "%s\n", history[history_counter]);
      // maybe this is not necessary
      // strcpy(line, history[history_counter]);
      // r_status = GO;
    } else if (r_status == GO){
      cmds_num = parse_line(line, cmds);
      history_counter = entered_cmd;
      // for (int i = 0; i < cmds_num; i++) {
      //   printf("%s\n",cmds[i]);
      // }

      if (history_counter >= HISTORY_SIZE) {
        // TODO: remove first command in history, and swap all command
        fprintf(stdout, "maximum history size\n");
        for (int i = 1; i < history_counter; i++) {
          strcpy(history[i-1], history[i]);
        }
        history_counter = HISTORY_SIZE - 1;
      }

      strcpy(history[history_counter], line);
      // check_commands(cmds, cmds_num, cmds_num);
      status = execute_commands(cmds, cmds_num);
      if (status == -1){
        break;
      }
      // clear previous command
      memset(*cmds, 0, MAX_CMD_NUM);
      for (int i = 0; i < MAX_CMD_NUM; i++) {
        memset(cmds[i], '\0', CMD_SIZE);
      }
      ++history_counter;
      ++entered_cmd;
    }
  }

  for (int i = 0; i < HISTORY_SIZE; i++) {
    free(history[i]);
  }
  free(history);
  for (int i = 0; i < cmds_num; i++) {
    free(cmds[i]);
  }
  free(cmds);
}

// read commands from command line
int read_commands(char *line){
  char ch;
  int position = 0;
  do {
    ch = fgetc(stdin);

    if (ch == '\033'){
      fgetc(stdin); // skip the [
      switch (fgetc(stdin)) {
        case 'A':
          return UP_KEY;
        case 'B':
          return DOWN_KEY;
      }
    } else {
      line[position] = ch;
      ++position;
      if (position == CMD_SIZE) {
        fprintf(stderr, "Too many commands have been entered. You can enter a command with %d characters\n",CMD_SIZE );
        return -1;
      }
    }

  } while(ch != '\n');

  return GO;
}

// split the line to gets every commands or tokens
int parse_line(char *line, char **cmds) {
  char *token;
  int cmd_num = 0;
  token = strtok(line, DELIMETERS);
  while(token != NULL) {
    strcpy(cmds[cmd_num], token);
    cmd_num++;
    if (cmd_num >= MAX_CMD_NUM) {
      fprintf(stderr, "You enter two many command. You can type 10 command in one line\n");
      exit(EXIT_FAILURE);
    }

    token = strtok(NULL, DELIMETERS);
  }
  return cmd_num;
}

// TODO: eger komutta pipe varsa bunu ona gore isleme sokman lazim
// TODO: hangi komuttan sonra pipe gelebilir ve hangi komuta o komutun ciktisi verilebilir ona gore isleme sokman lazim
//
int execute_commands(char **cmds, int cmd_num) {
  char *result_ls = NULL, *result_wc = NULL, *result_cat = NULL,
        result_pwd[MAX_STR_SIZE];
  int is_execute_ls = 0, is_execute_cat = 0, is_execute_wc = 0,
      is_execute_pwd = 0,
      print_ls = 0, print_cat = 0, print_wc = 0, print_pwd = 0;

  for (int i = 0; i < cmd_num; i++) {
    if (strcmp(cmds[i], LS) == 0) {
      if ( i + 1 < cmd_num){
        if ((strcmp(cmds[i+1],PIPE) == 0) ||
              (strcmp(cmds[i+1], RARROW) == 0) ||
                (strcmp(cmds[i+1], RRARROW) == 0)) {
          result_ls = execute_ls(NULL);
          print_ls = 0;
          printf("1) +++++++++++++++++\n");
        } else {
          result_ls = execute_ls(cmds[i+1]);
          i += 1; // because we execute ls with path
          if (i + 1 < cmd_num) { // ls path |
            printf("cmds[i+1]: %s\n",cmds[i+1] );
            if ((strcmp(cmds[i+1],PIPE) == 0) ||
                  (strcmp(cmds[i+1], RARROW) == 0) ||
                    (strcmp(cmds[i+1], RRARROW) == 0)) {
              printf("7) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
              print_ls = 0;
            } else{
              printf("8) ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
              print_ls = 1;
            }
          }else {
            printf("2) -------------------\n");
            print_ls = 1;
          }
        }
      } else {
        printf("3) ======================\n");
        result_ls = execute_ls(NULL);
        print_ls = 1;
      }
      is_execute_ls = 1;
    } else if (strcmp(cmds[i], EXIT) == 0){
      return -1;
    } else if ((strcmp(cmds[i], RARROW) == 0) || (strcmp(cmds[i], RRARROW) == 0)) {
      if (i + 1 < cmd_num) {
        if (is_execute_ls && result_ls != NULL) { // ls > filename
          if ((strcmp(cmds[i], RRARROW) == 0)) {
            execute_rredirect(result_ls, cmds[i + 1], APPEND_MODE);
          } else {
            execute_rredirect(result_ls, cmds[i + 1], REDIRECT_M);
          }
        } else if (is_execute_cat && result_cat != NULL) {
          if ((strcmp(cmds[i], RRARROW) == 0)) {
            execute_rredirect(result_cat, cmds[i + 1], APPEND_MODE);
          } else {
            execute_rredirect(result_cat, cmds[i + 1], REDIRECT_M);
          }
        } else if (is_execute_wc && result_wc != NULL) {
          if (strcmp(cmds[i], RARROW) == 0) {
            execute_rredirect(result_wc, cmds[i + 1], REDIRECT_M);
          } else {
            execute_rredirect(result_wc, cmds[i + 1], APPEND_MODE);
          }
        } else if (is_execute_pwd) {
          if (strcmp(cmds[i], RARROW) == 0) {
            execute_rredirect(result_pwd, cmds[i + 1], REDIRECT_M);
            print_pwd = 0;
          } else {
            execute_rredirect(result_pwd, cmds[i + 1], APPEND_MODE);
            print_pwd = 0;
          }
        }
        i += 1;
      }
    } else if (strcmp(cmds[i], PIPE) == 0){
      if (i + 1 < cmd_num) {
        if (strcmp(cmds[i+1], WC) == 0) {
          if (is_execute_ls && result_ls != NULL){ // ls | wc
            printf("4) **********************\n");
            print_ls = 0;
            print_wc = 1;
            result_wc = execute_wc(result_ls, PIPE_MODE);
            i += 1;
          } else {
            fprintf(stdout, "word count must take parameters.\n");
          }
        }
      }
    } else if (strcmp(cmds[i], WC) == 0) {
      if (i + 1 < cmd_num) {
        if (strcmp(cmds[i + 1], LARROW) == 0) { // wc < filename
          if (i + 2 < cmd_num){
            printf("5) $$$$$$$$$$$$$$$$$$$$$$$$$$\n");
            result_wc = execute_wc(cmds[i+2], FILE_MODE);
            i += 2;
            print_wc = 1;
          }
        } else { // wc filename
          printf("6) ########################\n");
          result_wc = execute_wc(cmds[i+1], FILE_MODE);
          i += 1;
          if (i + 1 < cmd_num) { // wc path >
            if ((strcmp(cmds[i+1], RARROW) == 0) ||
                    (strcmp(cmds[i+1], RRARROW) == 0)) {
              print_wc = 0;
            }
          }else {
            print_wc = 1;
          }
        }
        is_execute_wc = 1;
      } else {
        fprintf(stderr, "wc program must take argument.\n");
      }
    } else if (strcmp(cmds[i], PWD) == 0) {
      getcwd(result_pwd,sizeof(result_pwd));
      is_execute_pwd = 1;
      print_pwd = 1;
    } else if (strcmp(cmds[i], CAT) == 0) {
      fprintf(stderr, "NOT YET IMPLEMENTED\n");
    } else if (strcmp(cmds[i], HELP) == 0) {
      print_help();
    }

  }

  if (result_ls != NULL && print_ls) {
    fprintf(stdout, "%s\n", result_ls);
  }

  if (result_wc != NULL && print_wc) {
    fprintf(stdout, "%s\n", result_wc);
  }

  if (result_cat != NULL && print_cat) {
    fprintf(stdout, "%s\n",result_cat);
  }

  if (result_pwd[0] != '\0' && print_pwd) {
    fprintf(stdout, "%s\n", result_pwd);
  }

  if (result_ls != NULL){
    free(result_ls);
  }

  if (result_cat != NULL) {
    free(result_cat);
  }
  if (result_wc != NULL) {
    free(result_wc);
  }
  return 1;
}

void print_help() {
  fprintf(stdout, "--------------------------------------------------------\n");
  fprintf(stdout, "SUPPORTED COMMANDS: \n");
  fprintf(stdout, "ls; which will list file type, access rights, file size and file name of all files in the present working directory\n");
  fprintf(stdout, "pwd; which will print the present working directory\n");
  fprintf(stdout, "cd; which will change the present working directory to the location provided as argument\n");
  fprintf(stdout, "help; which will print the list of supported commands\n");
  fprintf(stdout, "cat; which will print on standard output the contents of the file provided to it as argument\n");
  fprintf(stdout, "wc; which will print on standard output the number of lines in the file provided to it as argument\n");
  fprintf(stdout, "exit; which will exit the shell\n");
  fprintf(stdout, "--------------------------------------------------------\n");
  fprintf(stdout, "SAMPLE USAGES:\n");
  fprintf(stdout, "ls | wc \n");
  fprintf(stdout, "ls <path> | wc\n");
  fprintf(stdout, "ls > <filename>\n");
  fprintf(stdout, "wc <filename>\n");
  fprintf(stdout, "wc < <filename>\n");
  fprintf(stdout, "pwd\n");
  fprintf(stdout, "pwd > <filename>\n");
  fprintf(stdout, "exit\n");
  fprintf(stdout, "--------------------------------------------------------\n");
  fprintf(stdout, "If you want to see history, push the <up> arrow key and push the enter\n");


}

void execute_rredirect(char *arg, char *filename, int mode) {
  int fd = -1, write_bytes = 0;

  if (mode == APPEND_MODE) {
    fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  } else {
    fd = open(filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  }
  if (fd == -1) {
    fprintf(stderr, "Failed to open file: %s\n",filename );
    return;
  }

  while(((write_bytes = write(fd, arg, strlen(arg))) == -1) && errno == EINTR);
  fprintf(stdout, "%d bytes write to file: %s\n", write_bytes, filename);
}

char *execute_wc(char *arg, int mode) {
  pid_t child_pid;
  int pipe_fd[2];

  if (pipe(pipe_fd)) {
    perror("Pipe failed, wc command could not execute: ");
    return NULL;
  }

  child_pid = fork();

  if (child_pid == -1) {
    perror("Fork failed. wc command could not execute: ");
    return NULL;
  } else if (child_pid == 0) { // child process
    // close read side, write fd[1]
    close(pipe_fd[0]);
    // redirect from stout to pipe write side
    dup2(pipe_fd[1], STDOUT);

    char *paramList[4];

    paramList[0] = "./wc";
    char str_mode[5];
    snprintf(str_mode,5,"%d",mode);
    paramList[1] = str_mode;
    paramList[2] = arg;
    paramList[3] = NULL;

    execvp("./wc", paramList);
    _exit(EXIT_SUCCESS);
  } else { // parent process
    close(pipe_fd[1]);
    char *buffer;
    buffer = (char *)calloc(MAX_STR_SIZE, sizeof(char));
    read(pipe_fd[0], buffer, MAX_STR_SIZE);

    return buffer;
  }
  return NULL;
}

char *execute_ls(char *arg) {
  pid_t chil_pid;
  int pipe_fd[2];

  if(pipe(pipe_fd)){
    perror("Pipe failed, ls command could not execute: ");
    return NULL;
  }

  chil_pid = fork();

  if (chil_pid == -1) {
    perror("Fork failed. ls command could not execute: ");
    return NULL;
  } else if (chil_pid == 0) { // child process
    // close read side, write fd[1]
    close(pipe_fd[0]);
    // redirect from stout to pipe write side
    dup2(pipe_fd[1], STDOUT);
    char *paramList[4];
    paramList[0] = "./ls";
    paramList[1] = "./";
    paramList[2] = NULL;
    if(arg != NULL){
      paramList[1] = arg;
    }

    execvp("./ls", paramList);

    _exit(EXIT_SUCCESS);
  } else { // parent process
    // close write side
    close(pipe_fd[1]);
    char *buffer;
    buffer = (char *)calloc(MAX_STR_SIZE, sizeof(char));
    read(pipe_fd[0], buffer, MAX_STR_SIZE);

    return buffer;
  }

  return NULL;
}



// TODO: do it later. This function should check the valid entered commands
int check_commands(const char **cmds, int cmd_num, int num) {

  if (num == 0) {
    return 0;
  }

  if (strcmp(cmds[0], LS) == 0) {

  }

  return -1;
}
