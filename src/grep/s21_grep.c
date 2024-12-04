#include "s21_grep.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Not enough arguments\n");
        return ERROR;
    }

    arg options = {0};
    int empty_pattern = 0;
    parse_options(argc, argv, &options, &empty_pattern);

    if (empty_pattern != -1 && strlen(options.pattern) > 0) {
        regex_t regex;
        if (check_error_reg(&options, &regex) == OK) {
            iter_files(argc, argv, options, regex, empty_pattern);
            regfree(&regex);
        }
    }

    return OK;
}

int check_error_reg(arg *arguments, regex_t *regex) {
    int flags = REG_EXTENDED | (arguments->i ? REG_ICASE : 0);
    int reerrcode = regcomp(regex, arguments->pattern, flags);
    if (reerrcode != 0) {
        char reerrbuf[1000];
        regerror(reerrcode, regex, reerrbuf, sizeof(reerrbuf));
        fputs(reerrbuf, stderr);
        fputc('\n', stderr);
    }
    return reerrcode;
}

void iter_files(int argc, char *argv[], arg arguments, regex_t regex, int empty_pattern) {
    for (int i = optind; i < argc; i++) {
        read_file(argv[i], &arguments, regex, argc - optind, empty_pattern, i);
    }
}

void read_file(const char *filename, arg *arguments, regex_t regex, int count_files, int empty_pattern, int file_index) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "No such file or directory: %s\n", filename);
        return;
    }

    char line[1024]; // Buffer for reading lines
    int count_lines = 0, line_number = 1, found_str = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove newline character if present
        line[strcspn(line, "\n")] = 0;

        if (output(line, arguments, regex, count_files, filename, line_number, empty_pattern)) {
            count_lines++;
            found_str = 1;
        }
        line_number++;
    }

    if (arguments->c) {
        if (count_files > 1 && !arguments->h) printf("%s:", filename);
        printf("%d\n", found_str ? 1 : 0);
    }

    if (arguments->l && found_str) printf("%s\n", filename);
    if (arguments->v && !arguments->c && !arguments->l && !arguments->i && (count_files == 1 || file_index + 1 - optind < count_files)) {
        printf("\n");
    }

    fclose(file);
}

int output(char *line, arg *arguments, regex_t regex, int count_files, const char *filename, int line_number, int empty_pattern) {
    regmatch_t pm;
    char *pline = line;
    int found_match = 0;

    while (regexec(&regex, pline, 1, &pm, 0) == 0) {
        if (arguments->o && !arguments->v && !arguments->c && !arguments->l) {
            for (int i = pm.rm_so; i < pm.rm_eo; i++) {
                putchar(pline[i]);
            }
            putchar('\n');
        }
        pline += pm.rm_eo;
        found_match = 1;
    }

    if (arguments->v) {
        found_match = !found_match;
    }

    if (!arguments->c) {
        if (found_match) {
            if ((!arguments->l && !arguments->o) || (arguments->o && arguments->v)) {
                if (count_files > 1 && !arguments->h) printf("%s:", filename);
                if (arguments->n) {
                    printf("%d:", line_number);
                }
                fputs(line, stdout);
            }
        } else if (strlen(arguments->pattern) == 0 || empty_pattern) {
            if (count_files > 1) printf("%s:", filename);
            fputs(line, stdout);
        }
    }

    return found_match;
}

int read_file_f(const char *filename, arg *arguments) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "No such file or directory: %s\n", filename);
        return -1;
    }

    char line[1024]; // Buffer for reading lines
    int empty_pattern = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0; // Remove newline character

        if (strlen(line) > 0) {
            if (strlen(arguments->pattern) > 0) {
                strcat(arguments->pattern, "|");
            }
            strcat(arguments->pattern, line);
        } else {
            empty_pattern = 1;
        }
    }

    fclose(file);
    return empty_pattern;
}

void parse_options(int argc, char *argv[], arg *arguments, int *empty_pattern) {
    int option;
    while ((option = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
        switch (option) {
            case 'e':
                arguments->e = 1;
                if (strlen(arguments->pattern) > 0) strcat(arguments->pattern, "|");
                strcat(arguments->pattern, optarg);
                break;
            case 'i':
                arguments->i = 1;
                break;
            case 'v':
                arguments->v = 1;
                break;
            case 'c':
                arguments->c = 1;
                break;
            case 'l':
                arguments->l = 1;
                break;
            case 'n':
                arguments->n = 1;
                break;
            case 'h':
                arguments->h = 1;
                break;
            case 's':
                arguments->s = 1;
                break;
            case 'f':
                arguments->f = 1;
                *empty_pattern = read_file_f(optarg, arguments);
                break;
            case 'o':
                arguments->o = 1;
                break;
            default:
                break;
        }
    }

    if (!arguments->e && !arguments->f && optind < argc) {
        strcat(arguments->pattern, argv[optind]);
        optind++;
    }
} 