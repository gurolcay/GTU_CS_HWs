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


#define MAX_STR_SIZE  2048

char* readable_fs(double size/*in bytes*/, char *buf);


int main(int argc, char const *argv[]) {
  DIR *mydir;
  struct dirent *myfile;
  struct stat fstat;
  char dir_name[MAX_STR_SIZE];
  char buf[10];
  char isdir, irusr, iwusr, ixusr, irgrp, iwgrp, ixgrp, iroth, iwoth, ixoth;

  memset(dir_name, '\0', MAX_STR_SIZE);

  printf("argc: %d\n",argc );

  if (argc > 1){
    strcpy(dir_name, argv[1]);
    printf("dir_name: %s\n",dir_name);
  } else if (argc == 1) {
    strcpy(dir_name, "./");
  }

  mydir = opendir(dir_name);
  if (mydir == NULL) {
    perror("ls command is fail: ");
    return -1;
  }

  while ((myfile = readdir(mydir)) != NULL) {
    char *result;
    result = (char *)calloc(MAX_STR_SIZE, sizeof(char));

    stat(myfile->d_name, &fstat);
    if (S_ISDIR(fstat.st_mode)) isdir = 'd'; else isdir = '-';
    if (fstat.st_mode & S_IRUSR) irusr = 'r'; else irusr = '-';
    if (fstat.st_mode & S_IWUSR) iwusr = 'w'; else iwusr = '-';
    if (fstat.st_mode & S_IXUSR) ixusr = 'x'; else ixusr = '-';
    if (fstat.st_mode & S_IRGRP) irgrp = 'r'; else irgrp = '-';
    if (fstat.st_mode & S_IWGRP) iwgrp = 'w'; else iwgrp = '-';
    if (fstat.st_mode & S_IXGRP) ixgrp = 'x'; else ixgrp = '-';
    if (fstat.st_mode & S_IROTH) iroth = 'r'; else iroth = '-';
    if (fstat.st_mode & S_IWOTH) iwoth = 'w'; else iwoth = '-';
    if (fstat.st_mode & S_IXOTH) ixoth = 'x'; else ixoth = '-';

    snprintf(result, MAX_STR_SIZE, "%c%c%c%c%c%c%c%c%c%c\t%s\t\t%s",
                                              isdir, irusr, iwusr, ixusr, irgrp,
                                              iwgrp, ixgrp, iroth, iwoth, ixoth,
                                              readable_fs(fstat.st_size, buf), myfile->d_name);
    fprintf(stdout, "%s\n",result);
    free(result);
  }
  return 0;
}

// http://programanddesign.com/cpp/human-readable-file-size-in-c/
char* readable_fs(double size/*in bytes*/, char *buf) {
    int i = 0;
    const char* units[] = {"B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    while (size > 1024) {
        size /= 1024;
        i++;
    }
    sprintf(buf, "%.*f%s", i, size, units[i]);
    return buf;
}
