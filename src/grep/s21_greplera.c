#include "s21_greplera.h"

int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr, "error: not enough arguments");
  } else {
    arguments* arg = calloc(1, sizeof(arguments));
    if (!arg) {
      fprintf(stderr, "something has failed :(");
    } else {
      argumentparser(argc, argv, arg);
      for (int i = optind; i < argc; i++) {
        const char* filename = argv[i];
        FILE* f = fopen(filename, "r");

        if (!f) {
          fprintf(stderr, "something is wrong with the file :(");
        } else {
          linescanning(argc, arg, f, filename);
        }

        fclose(f);
      }
    }
    free(arg);
  }
  return 0;
}

void linescanning(int argc, arguments* arg, FILE* f, const char* filename) {
  size_t line_memory = 0;
  char* line = NULL;
  regex_t regex;
  int matchcounter = 0;
  int linenumber = 1;
  int flags = REG_EXTENDED;

  if (arg->i) flags |= REG_ICASE;
  if (regcomp(&regex, arg->patterns, flags) != 0) {
    fprintf(stderr, "something has failed :(");
    return;
  }

  ssize_t readcount;
  while ((readcount = getline(&line, &line_memory, f)) != -1) {
    int check = searching(line, arg, regex, filename, &matchcounter, argc);
    if (arg->n && !check) {
      if (argc - optind > 1)
        printf("%s:%d:%s", filename, linenumber, line);
      else
        printf("%d:%s", linenumber, line);
    }
    linenumber++;
  }

  if (arg->l) {
    if (matchcounter > 0) {
      printf("%s\n", filename);
    }
  }
  if (arg->c) {
    if (argc > 4) {
      printf("%s:%d\n", filename, matchcounter);
    } else {
      printf("%d\n", matchcounter);
    }
  }

  if (line != NULL) free(line);
  regfree(&regex);
}

int searching(char* line, arguments* arg, regex_t regex, const char* filename,
              int* matchcounter, int argc) {
  regmatch_t pmatch[100];
  int check = regexec(&regex, line, 0, pmatch, 0);
  if (check == 0) (*matchcounter)++;
  sorting(arg, check, line, filename, argc);
  return check;
}

arguments* argumentparser(int argc, char** argv, arguments* arg) {
  arg->e = arg->i = arg->v = arg->c = arg->l = arg->n = 0;
  arg->patterns[0] = '\0';

  struct option long_options[] = {
      {"pattern", no_argument, NULL, 'e'},
      {"invert-match", no_argument, NULL, 'v'},
      {"ignore-case", no_argument, NULL, 'i'},
      {"count", no_argument, NULL, 'c'},
      {"files-with-matches", no_argument, NULL, 'l'},
      {"line-number", no_argument, NULL, 'n'},
      {0, 0, 0, 0}};

  int opt;

  while ((opt = getopt_long(argc, argv, "e:ivcln", long_options, NULL)) != -1) {
    switch (opt) {
      case 'e':
        arg->e = 1;
        strcat(arg->patterns, optarg);
        strcat(arg->patterns, "|");
        break;
      case 'i':
        arg->i = 1;
        break;
      case 'v':
        arg->v = 1;
        break;
      case 'c':
        arg->c = 1;
        break;
      case 'l':
        arg->l = 1;
        break;
      case 'n':
        arg->n = 1;
        break;
      default:
        fprintf(stderr,
                "usage: grep [-eivcln] [-e pattern] [-f file] [pattern] [file "
                "...]");
        break;
    }
  }

  size_t len = strlen(arg->patterns);
  if (len > 0 && arg->patterns[len - 1] == '|') {
    arg->patterns[len - 1] = '\0';
  }
  if (!arg->e && optind < argc) {
    strcat(arg->patterns, argv[optind]);
    optind++;
  }

  return arg;
}

void sorting(arguments* arg, int check, char* line, const char* filename,
             int argc) {
  if (arg->v) check = !check;
  if (!arg->l && !arg->n && !arg->c) {
    if (check == 0) {
      if (!arg->n) {
        if ((argc - optind) > 1) {
          printf("%s:%s", filename, line);
        } else {
          printf("%s", line);
        }
      }
    }
  }
}
