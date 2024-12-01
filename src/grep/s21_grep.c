#include "s21_grep.h"

void lineGetter(int argc, flags* item, FILE* file, const char* filename) {
  char* line = NULL;
  size_t lineMemory = 0;
  regex_t regex;
  int linenAmount = 1;
  int matchAmount = 0;
  int regexMode = REG_EXTENDED;

  if (item->i) regexMode = regexMode | REG_ICASE;
  if (regcomp(&regex, item->patterns, regexMode) != 0) {
    fprintf(stderr, "invalid patterns || pattern don't exist");
    return;
  }

  ssize_t readcount;
  while ((readcount = getline(&line, &lineMemory, file)) != -1) {
    int check = handleSearch(line, item, regex, filename, &matchAmount, argc);
    if (item->n && !check) {
      if (argc - optind > 1)
        printf("%s:%d:%s", filename, linenAmount, line);
      else
        printf("%d:%s", linenAmount, line);
    }
    linenAmount++;
  }

  if (item->l) {
    if (matchAmount > 0) {
      printf("%s\n", filename);
    }
  }
  if (item->c) {
    if (argc > 4) {
      printf("%s:%d\n", filename, matchAmount);
    } else {
      printf("%d\n", matchAmount);
    }
  }

  if (line != NULL) free(line);
  regfree(&regex);
}

int handleSearch(char* line, flags* item, regex_t regex, const char* filename,
              int* matchAmount, int argc) {
  regmatch_t pmatch[100];
  int check = regexec(&regex, line, 0, pmatch, 0);
  if (check == 0) (*matchAmount)++;
  handleSort(item, check, line, filename, argc);
  return check;
}

void handleInitializeFlags(flags* item) {
  item->e = item->i = item->v = item->c = item->l = item->n = 0;
  item->patterns[0] = '\0';
}

void parserFlags(int argc, char** argv, flags* item) {
  int opt;

  while ((opt = getopt(argc, argv, "e:ivcln")) != -1) {
    switch (opt) {
      case 'e':
        item->e = 1;
        strcat(item->patterns, optarg);
        strcat(item->patterns, "|");
        break;
      case 'i':
        item->i = 1;
        break;
      case 'v':
        item->v = 1;
        break;
      case 'c':
        item->c = 1;
        break;
      case 'l':
        item->l = 1;
        break;
      case 'n':
        item->n = 1;
        break;
      default:
        fprintf(stderr, "invalid flag write grep --help");
        break;
    }
  }

  size_t length = strlen(item->patterns);
  if (length > 0 && item->patterns[length - 1] == '|') {
    item->patterns[length - 1] = '\0';
  }
  if (!item->e && optind < argc) {
    strcat(item->patterns, argv[optind]);
    optind++;
  }
}

void handleSort(flags* item, int check, char* line, const char* filename,
             int argc) {
  if (item->v) check = !check;
  if (!item->l && !item->n && !item->c) {
    if (check == 0 && !item->n) {
        if ((argc - optind) > 1) {
          printf("%s:%s", filename, line);
        } else {
          printf("%s", line);
        }
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr, "error: not enough flags");
  } else {
    flags* item = calloc(1, sizeof(flags));
    handleInitializeFlags(item);
    if (!item) {
      fprintf(stderr, "something has failed :(");
    } else {
      parserFlags(argc, argv, item);
      for (int i = optind; i < argc; i++) {
        const char* filename = argv[i];
        FILE* file = fopen(filename, "r");

        if (!file) {
          fprintf(stderr, "something is wrong with the file :(");
        } else {
          lineGetter(argc, item, file, filename);
        }

        fclose(file);
      }
    }
    free(item);
  }
  return 0;
}