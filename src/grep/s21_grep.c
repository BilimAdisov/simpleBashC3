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
    char errbuf[100];
    regerror(regcomp(&regex, item->patterns, regexMode), &regex, errbuf, sizeof(errbuf));
    fprintf(stderr, "invalid patterns");
    exit(EXIT_FAILURE);
  }

  ssize_t readAmount;
  while ((readAmount = getline(&line, &lineMemory, file)) != -1) {
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

int handleSearch(char* line, flags* items, regex_t regex, const char* filename,
                 int* matchAmount, int argc) {
  regmatch_t pmatch[100];
  int check = regexec(&regex, line, 0, pmatch, 0);
  if (check == 0) (*matchAmount)++;
  handleSort(items, check, line, filename, argc);
  return check;
}

void handleInitializeFlags(flags* item) {
  item->e = item->i = item->v = item->c = item->l = item->n = 0;
  item->patterns[0] = '\0';
}

void parserFlags(int argc, char** argv, flags* items) {
  handleInitializeFlags(items);
  int opt;

  while ((opt = getopt(argc, argv, "e:ivcln")) != -1) {
    switch (opt) {
      case 'e':
        items->e = 1;
        strcat(items->patterns, optarg);
        strcat(items->patterns, "|");
        break;
      case 'i':
        items->i = 1;
        break;
      case 'v':
        items->v = 1;
        break;
      case 'c':
        items->c = 1;
        break;
      case 'l':
        items->l = 1;
        break;
      case 'n':
        items->n = 1;
        break;
      default:
        fprintf(stderr, "invalid flag write grep --help");
        break;
    }
  }

  size_t length = strlen(items->patterns);
  if (length > 0 && items->patterns[length - 1] == '|') {
    items->patterns[length - 1] = '\0';
  }
  if (!items->e && optind < argc) {
    strcat(items->patterns, argv[optind]);
    optind++;
  }
}

void handleSort(flags* items, int check, char* line, const char* filename,
                int argc) {
  if (items->v) check = !check;
  if (!items->l && !items->n && !items->c) {
    if (check == 0 && !items->n) {
        if ((argc - optind) > 1) {
          printf("%s:%s", filename, line);
        } else {
          printf("%s", line);
        }
    }
  }
}

int main(int argc, char* argv[]) {
  flags item;
  if (argc > 1) {
    parserFlags(argc, argv, &item);
    for (int i = optind; i < argc; i++) {
      const char* filename = argv[i];
      FILE* file = fopen(filename, "r");

      if (!file) {
        fprintf(stderr, "file dont opened :(>>");
      } else {
        lineGetter(argc, &item, file, filename);
        fclose(file);
      }
    }
  } else {
    fprintf(stderr, "error: not enough flags");
  }
  return 0;
}
