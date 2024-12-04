#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int e, i, v, c, l, n, h, s, f, o;
    char pattern[1024];
} arg;

#define OK 0
#define ERROR 1

void read_file(const char *filename, arg *arguments, regex_t regex, int count_files, int empty_pattern, int file_index);
int read_file_f(const char *filename, arg *arguments);
void parse_options(int argc, char *argv[], arg *arguments, int *empty_pattern);
int output(char *line, arg *arguments, regex_t regex, int count_files, const char *filename, int line_number, int empty_pattern);
void iter_files(int argc, char *argv[], arg arguments, regex_t regex, int empty_pattern);
int check_error_reg(arg *arguments, regex_t *regex);

#endif // S21_GREP_H