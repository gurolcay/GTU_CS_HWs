#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PIPE_MODE   100   // wc program takes input from pipe
#define FILE_MODE   200   // wc program takes filename as input

char *wc_func_from_file();
char *wc_func();
int main(int argc, char const *argv[]) {

  int bytes = 0, words = 0, new_line = 0;
  int white_space = 0;
  if (argc != 3) {
    fprintf(stderr, "wc program must take parameters\n");
    return -1;
  }
  
  if (PIPE_MODE == (int)strtol(argv[1], NULL, 10)) {
    int length = strlen(argv[2]);
    char *document = argv[2];
    // int length = strlen(document);
    for (int i = 0; i < length; i++) {
      if (document[i] == ' ' || document[i] == '\t') {
        white_space = 1;
      }
      else if (document[i] == '\n') {
        ++new_line;
      } else {
        if (white_space) {
          ++words;
        }
      }
      ++bytes;
    }
  } else {
    FILE *file_p = fopen(argv[2], "r");

    if (file_p == NULL){
      fprintf(stderr, "Can not find: %s\n",argv[2]);
      return -1;
    }
    char ch = fgetc(file_p);
    while(ch != EOF){
      ++bytes;
      if (ch == ' ' || ch == '\t'){
        white_space = 1;
      } else if (ch == '\n'){
        ++new_line;
      } else {
        if (white_space) {
          ++words;
          white_space = 0;
        }
      }
      ch = fgetc(file_p);
    }

    fclose(file_p);
  }

  fprintf(stdout, "New Lines\tWords\tBytes\t\n%d\t\t%d\t%d\n",new_line,words,bytes);
  return 0;
}
