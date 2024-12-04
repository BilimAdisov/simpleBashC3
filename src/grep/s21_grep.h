
#ifndef GREP_H
#define GREP_H

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
void lineGetter(int argc, flags* item, FILE* file, const char* filename);
void parserFlags(flags* items, int argc, char *argv[]);
int handleSearch(char* line, flags* items, regex_t regex, const char* filename,
                 int* matchAmount, int argc);
void handleSort(flags* items, int check, char* line, const char* filename,
                int argc);
#endif