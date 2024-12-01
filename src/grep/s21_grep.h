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

typedef struct fFlags {
  int e, i, v, c, l, n;
  char patterns[MAXPATTERNS * (MAXPATTERNLENGTH + 1)];
} flags;

void handleInitializeFlags(flags* item);
void lineGetter(int argc, flags* arg, FILE* file, const char* filename);
void parserFlags(int argc, char** argv, flags* arg);
int handleSearch(char* line, flags* arg, regex_t regex, const char* filename,
              int* matchcounter, int argc);
void handleSort(flags* arg, int check, char* line, const char* filename, int argc);