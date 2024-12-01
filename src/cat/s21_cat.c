#include "s21_cat.h"

void initFlags(flags *items) {
    items->temp_f.n = items->temp_f.b = items->temp_f.s = items->temp_f.s1 = items->temp_f.text = 0;
}

void initVariables(flags *items) {
    items->b = items->e = items->v = items->n = items->s = items->t = items->files = 0;

}

int handleGetFiles(char *argsState[], int argc, char *argv[]) {
    int count = 0;
    for(int i = 1; i < argc; i++) {
        if(argv[i][0] != '-') {
            argsState[count] = argv[i];
            count++;
        }
    }
    return count;
}

void handleCatFiles(flags *items, int argc, char *argv[]) {
    FILE *files[argc];

    for(int i = 0; i < argc; i++) {
        files[i] = fopen(argv[i], "r");
    }

    handleOutputStrems(items, files, argc);

    for(int i = 0; i < argc; i++) {
        fclose(files[i]);
    }
}

void handleOutputStrems(flags *items, FILE *streams[], int amount) {
    int string, currentStream = 0;

    initVariables(items);

    if(items->n || items->b) {
        caseNB(items, streams, amount, &currentStream);
    }
    while((string = handleGetStreams(streams, amount, &currentStream)) != EOF) {
        if(items->n && caseN(items, string)) {
            continue;
        }
        if(items->b) {
            caseB(items, string);
        }
        if(items->s && caseS(items, string)) {
            continue;
        }
        if(items->v || items->e || items->t) {
            caseVET(items, string);
        } else {
            putchar(string);
        }
    }
}

int handleGetStreams(FILE *streams[], int amount, int *currentStream) {
    int ch;
    while(1) {
        if(streams[*currentStream] == NULL) {
            perror("file dont exist");
            currentStream++;
            continue;
        }
        ch = fgetc(streams[*currentStream]);
        if(ch == EOF) {
            if(*currentStream < amount - 1) {
                currentStream++;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    return ch;
}

void handleEndcoding(int ch) {
    if(ch != 10) {
        if(ch >= 128) {
            ch -= 128;
            putchar('M');
            putchar('-');
        }
        if(ch < 32 || ch == 127) {
            putchar('^');
            ch ^= 0x40;
        }
        putchar(ch);
    } else if(ch == 10) {
        putchar('$');
        putchar('\n');
    }
}

void caseVET(flags *items, int string) {
    if(items->v && (string < 32 || string > 126) && (string != 9 && string > 126) && (string != 9 && string != 10)) {
        handleEndcoding(string);
    } else if(items->e && items->v && (string < 32 || string > 126) && (string != 9)) {
        handleEndcoding(string);
    } else if(items->e && !items->v && string  == 10) {
        handleEndcoding(string);
    } else if(items->t && items->v && (string < 32 || string > 126) && (string != 10)) {
        handleEndcoding(string);
    } else if(items->t && string == 9) {
        handleEndcoding(string);
    } else {
        putchar(string);
    }
}

int caseS(flags *items, int string) {
    int isContinue = 0;
    if(string == '\n' && !items->temp_f.s) {
        items->temp_f.s = 1;
    } else if(string == '\n' && items->temp_f.s) {
        if(!items->temp_f.s1 && items->temp_f.text) {
            items->temp_f.s1 = 1;
            putchar(string);
        }
        isContinue = 1;
    } else if(string != '\n') {
        items->temp_f.s = 0;
        items->temp_f.s1 = 0;
        items->temp_f.text = 1;
    }
    return isContinue;
}

void caseB(flags *items, int string) {
    if(string == '\n' && !items->temp_f.b) {
        items->temp_f.b = 1;
    } else if(string != '\n' && items->temp_f.b) {
        printf("%6d\t", items->line);
        items->line++;
        items->temp_f.b = 0;
    }
}

int caseN(flags *items, int string) {
    int isContinue = 0;
    if(items->temp_f.n && string == '\n') {
        printf("%6d\t", items->line);
        items->line++;
        putchar(string);
        isContinue = 1;
    }
    if(string != '\n' && items->temp_f.n && !isContinue) {
        printf("%6d\t", items->line);
        items->temp_f.n = 0;
    } else if(string == '\n' && !items->temp_f.n && !isContinue) {
        items->line++;
        items->temp_f.n = 1;
    }
    return isContinue;
}

void caseNB(flags *items, FILE *streams[], int amount, int *currentStream) {
    int stream;
    if(items->n && items->files) {
        printf("%6d\t", items->line);
    } else if(items->n && !items->files && (stream = handleGetStreams(streams, amount, currentStream))) {
        printf("%6d\t", items->line);
        if(stream == '\n') {
            items->temp_f.n = 1;
            items->line++;
        }
    }
    if(items->b && (stream = handleGetStreams(streams, amount, currentStream)) != '\n') {
        printf("%6d\t", items->line);
        items->line++;
        putchar(stream);
    } else if(stream == '\n') {
        items->temp_f.b = 1;
        putchar(stream);
    }
}

void handleParseFlags(flags *items, int argc, char *argv[]) {
    int opt;
    items->none = 0;

    while((opt = getopt(argc, argv, "beEvnstT")) != -1) {
        switch(opt) {
            case 'b':
                items->b = 1;
                items->line = 1;
                items->none = 1;
                 break;
            case 'E':
                items->e = 1;
                items->none = 1;
                 break;
            case 'e':
                items->e = 1;
                items->v = 1;
                items->none = 1;
                 break;
            case 'v':
                items->v = 1;
                items->none = 1;
                 break;
            case 'n':
                items->n = 1;
                items->line = 1;
                items->none = 1;
                 break;
            case 's':
                items->s = 1;
                items->none = 1;
                 break;
            case 't':
                items->t = 1;
                items->v = 1;
                items->none = 1;
                 break;
            case 'T':
                items->t = 1;
                items->none = 1;
                 break;
            case '?':
                fprintf(stderr, "invalid arguments, please try cat --help");
                break;
        }
    }
}

int main (int argc, char *argv[]) {
    flags items;
    initFlags(&items);

    if(argc > 1) {
        char *argcState[argc - 1];
        items.files = handleGetFiles(argcState, argc, argv);
        if(items.none) {
            if(items.files != 0) {
                handleCatFiles(&items, items.files, argcState);
            } else {
                FILE *streams[1] = {stdin};
                handleOutputStrems(&items, streams, 1);
            }
        } else {
            handleCatFiles(&items, items.files, argcState);
        }
    } else {
        FILE *streams[1] = {stdin};
        handleOutputStrems(&items, streams, 1);
    }
    return 0;
}