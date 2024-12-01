#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct flags_alt {
  int n, b, s, s1, text;
} flags_alt;

typedef struct flags {
  int b, e, v, n, s, t, line, none, files;
  flags_alt temp_f;
} flags;

void handleEndcoding(int ch);
void initFlags(flags *item);
void initVariables(flags *items);
void caseB(flags *items, int string);
void caseVET(flags *items, int string);
void handleCatFiles(flags *items, int argc, char *argv[]);
void handleParseFlags(flags *items, int argc, char *argv[]);
void handleOutputStrems(flags *items, FILE *streams[], int amount);
void caseNB(flags *items, FILE *streams[], int amount, int *currentStream);
int caseN(flags *items, int string);
int caseS(flags *items, int string);
int handleGetStreams(FILE *streams[], int amount, int *currentStream);

#endif