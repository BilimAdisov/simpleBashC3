#define _POSIX_C_SOURCE 200809L
#define GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define MAXPATTERNLENGTH 100
#define MAXPATTERNS 1000

typedef struct farguments {
  int e, i, v, c, l, n;
  char patterns[MAXPATTERNS * (MAXPATTERNLENGTH + 1)];
} arguments;

void linescanning(int argc, arguments* arg, FILE* f, const char* filename);
arguments* argumentparser(int argc, char** argv, arguments* arg);
int searching(char* line, arguments* arg, regex_t regex, const char* filename,
              int* matchcounter, int argc);
void sorting(arguments* arg, int check, char* line, const char* filename,
             int argc);