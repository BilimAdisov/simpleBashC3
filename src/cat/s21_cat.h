#ifndef CAT
#define CAT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

typedef struct flags_alg {
  int n,b,s,s1,text;
}flags_alg; // флаги алгоритмов работы определенных флагов 

typedef struct flags {
  int b, e, v, n, s, t, line, none, files;
  flags_alg temp_f;
} flags; // аргументы функции
/*буквы - флаги
  line -  количество строк
  none - наличие флагов 1 есть - 0 нет
  files - наличие файлов*/


#endif