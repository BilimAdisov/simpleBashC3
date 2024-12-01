#include "s21_cat.h"

void init_flags_counter(flags *counter) {
  counter->temp_f.n = counter->temp_f.b = counter->temp_f.s =
      counter->temp_f.s1 = counter->temp_f.text = 0;
}

void init_flags(flags *flag) {
  flag->b = flag->e = flag->v = flag->n = flag->s = flag->t = flag->files = 0;
}

void encode_with_notation(int ch) {
  if (ch != 10) {  // в случае где кодировка работает на все символы с
                   // исключением табов и переноса строк
    if (ch >= 128) {
      ch -= 128;
      putchar('M');
      putchar('-');
    }

    if (ch < 32 || ch == 127) {
      putchar('^');
      ch ^= 0x40;
    }

    putchar(ch);
  } else if (ch == 10) {  // обработка переноса строки
    putchar('$');
    putchar('\n');
  }
}

int streams_getc(FILE *streams[], int count, int *stream_i) {
  // аналог fgetc  который работает на потоки файлов
  int ch;

  while (1) {
    if (streams[*stream_i] == NULL) {  // если не удалось открыть файл
      perror("No such file or directory");  // выводим ошибку
      ++*stream_i;  // переходим к  след потоку
      continue;
    }

    ch = fgetc(streams[*stream_i]);

    if (ch == EOF) {
      if (*stream_i <
          count - 1) {  // если еще остались файлы то меняется индекс потока/ -1
        // потому то самый последний элемент имеет индекс count -1, что бы не
        // выйти за пределы
        ++*stream_i;
      } else {
        break;  // возвращает если это последний файл EOF
      }
    } else {
      break;  // возвращает символ который получили
    }
    /*
    данный цикел всегда возвращает символ если только он не EOF  и при этом файл
    удалось открыть
    */
  }

  return ch;
}

void first_line_nb(flags *cats, FILE *streams[], int count, int *stream_i) {
  int temp;

    
  if (cats->n && cats->files) {  // нужен для самой первой строки так как она не начинается с
                  // переноса при флаге n
    printf("%6d\t", cats->line);
  }else if( cats->n && !cats->files && (temp = streams_getc(streams, count, stream_i))){
    printf("%6d\t", cats->line);

    if(temp == '\n'){
      cats->temp_f.n = 1;
      cats->line++;
    }
    /*отдельный цикл  для обработки cat -n без файла нужен запрос ввода то бы блокировать вывод в поток номер.
     в слуае если перенос строки - добавить флаги (то бы не было пробела лишнего) - если 2 переноса подряд*/
  }

  if (cats->b && (temp = streams_getc(streams, count, stream_i)) !=
                     '\n') {  // нужен при самой первой строке для флага b
    printf("%6d\t", cats->line);
    cats->line++;
    putchar(temp);
    /*если строка не пустая  выводится счетчик*/
  } else if (temp == '\n') {
    cats->temp_f.b = 1;
    putchar(temp);
    /*если это перенос он выводится*/
  }
}

int case_n(flags *cats, int temp) {
  /* ФЛАГ N (ЗА ИСКЛЮЕНИЕМ ПЕРВОЙ СТРОКИ)*/
  int continue_itteration = 0;
  // нужна для пропуска лишней итерации там где нужно

  if (cats->temp_f.n && temp == '\n') {  // флаг есть и символ перенос
    printf("%6d\t", cats->line);
    cats->line++;
    putchar(temp);
    continue_itteration = 1;
  } /* на случай если идут несколько переносов строк подряд
   если ее отключить по сути будет flag b*/

  if (temp != '\n' && cats->temp_f.n &&
      !continue_itteration) {  // символ не пренос  и флаг есть
    printf("%6d\t", cats->line);
    cats->temp_f.n = 0;

    /*на случай когда оказывается что следующая на следующей строке есть
    какие-то символы по сути фактический вывод, если символов нет - цифры не
    будет*/

  } else if (temp == '\n' && !cats->temp_f.n && !continue_itteration) {
    // если символ переноса и флага нет
    cats->line++;
    cats->temp_f.n = 1;

    /*
    увеличисвается счетчик строк
    ставится флаг
    перенос на следующую строку осуществится автоматом дальше в коде*/
  }

  return continue_itteration;  // если 0 то цикл пойдет дальше, 1 - цикл
                               // пропустится
}

void case_b(flags *cats, int temp) {
  if (temp == '\n' && !cats->temp_f.b) {  // если перенос строки и флага нет
    cats->temp_f.b = 1;  // ставим флаг что символ переноса есть

    /* если встречается перенос  ставим флаг и переносим*/
  } else if (temp != '\n' &&
             cats->temp_f.b) {  // если символ не перенос строки и флаг есть
    printf("%6d\t", cats->line);
    cats->line++;
    cats->temp_f.b = 0;
  } /* если символ не переносный  и есть флаг - печать счетсика*/
}

int case_s(flags *cats, int temp) {
  int continue_itteration = 0;
  // нужна для пропуска лишней итерации там где нужно

  if (temp == '\n' && !cats->temp_f.s) {  // флаг  b
    cats->temp_f.s = 1;  // ставим флаг что символ переноса есть

    /* если символ переносный  и флага нет ставим флаг*/
  } else if (temp == '\n' && cats->temp_f.s) {  // флаг s
    if (!cats->temp_f.s1 && cats->temp_f.text) {
      cats->temp_f.s1 = 1;
      putchar(temp);
    } /* нужен для 1 строки между текстом*/
    continue_itteration = 1;
    /* если символ переносный и перенос уже был либо условие, либо пропуск (так
     * как перенос уже был)*/
  } else if (temp != '\n') {
    cats->temp_f.s = 0;
    cats->temp_f.s1 = 0;
    cats->temp_f.text = 1;
    /*если встретили текст  сбрасываем флаги*/
  }

  return continue_itteration;  // если 0 то цикл пойдет дальше, 1 - цикл
                               // пропустится
}

void cases_v_e_t(flags *cats, int temp) {
  if (cats->v && (temp < 32 || temp > 126) &&
      (temp != 9 && temp != 10)) {  // только флаг v
    // то же что и флаг e кроме $ за перенос
    encode_with_notation(temp);
  } else if (cats->e && cats->v && (temp < 32 || temp > 126) &&
             (temp != 9)) {  // флаг  e
    //(подразумевает и флаг E но скрытые символы (кроме табуляции))
    encode_with_notation(temp);
  } else if (cats->e && !cats->v && temp == 10) {  // флаг E
    //(обрабатывает только переносы строк как $)
    encode_with_notation(temp);
  } else if (cats->t && cats->v && (temp < 32 || temp > 126) &&
             (temp != 10)) {  // флаг t
    // то же все символы которые невидимы  кроме \n
    encode_with_notation(temp);
  } else if (cats->t && temp == 9) {  // флаг T
    // обрабатывает тооолько \t
    encode_with_notation(temp);
  } else {
    putchar(temp);
  }
}

void output_streams(flags *cats, FILE *streams[], int count) {  // вывод в поток
  int temp, stream_i = 0;
  // stream_i - индекс текущего потока / temp - символ в  потоке

  init_flags_counter(cats);  // инициализация флагов

  if (cats->n || cats->b) {
    /* нумерация  самых первых строк для флагов n-b*/
    first_line_nb(cats, streams, count, &stream_i);
  }

  while ((temp = streams_getc(streams, count, &stream_i)) != EOF) {
    if (cats->n && case_n(cats, temp)) {  // флаг n
      continue;  // если вывод символа уже был
    }

    if (cats->b) {  // флаг b
      case_b(cats, temp);
    }

    if (cats->s && case_s(cats, temp)) {  // флаг s
      continue;  // если вывод символа уже был
    }

    if (cats->v || cats->e || cats->t) {  // флаг -v - e -t -E -T
      cases_v_e_t(cats, temp);
    } else {
      putchar(temp);  // просто вывод для других флагов (не -v -e -t -E -T)
    }
  }
}

void handle_flags(flags *cats, int argc,
                  char *argv[]) {  // проверяет аргументы на флаги
  int opt;
  cats->none = 0;

  while ((opt = getopt(argc, argv, "beEvnstT")) != -1) {
    switch (opt) {
      case 'b':
        cats->b = 1;
        cats->line = 1;
        cats->none = 1;
        break;
      case 'E':  // применяется только Е
        cats->e = 1;
        cats->none = 1;
        break;
      case 'e':  // применяются оба флага (e-v) поэтому проварилвается оба
        cats->e = 1;
        cats->v = 1;
        cats->none = 1;
        break;
      case 'v':
        cats->v = 1;  // применяется только v
        cats->none = 1;
        break;
      case 'n':
        cats->n = 1;
        cats->line = 1;
        cats->none = 1;
        break;
      case 's':
        cats->s = 1;
        cats->none = 1;
        break;
      case 't':
        cats->t = 1;
        cats->v = 1;
        cats->none = 1;
        break;
      case 'T':
        cats->t = 1;
        cats->none = 1;
        break;
      case '?':  // на случай если опция заведомо не существует
        fprintf(stderr,
                "Try 'cat --help' for more information.");  // в поток ошибок
                                                            // выводи текст и
                                                            // cимвол
        // optopt - глобальная переменная которая хранит конкретно символ
        // который не входит в список шаблонов
        break;
    }
  }
}

void cat_with_files(flags *cats, int argc, char *argv[]) {
  /*открывает все файлы в массив потоков который подается в функцию вывода*/

  FILE *files[argc];  // создается массив потоков  на кол-во файлов

  for (int i = 0; i < argc; i++) {
    files[i] = fopen(argv[i], "r");  // открываются потоки в массив
  }

  output_streams(cats, files, argc);  // передаем все потоки вфункцию обработки

  for (int i = 0; i < argc; i++) {
    fclose(files[i]);  // закрываем все потоки
  }
}

int get_pos_args(char *pos_args[], int argc, char *argv[]) {
  // записывает кол-во путей к файлам в массив исклюая флаг
  int count = 0;  // счетчик

  for (int i = 1; i < argc; ++i) {  // цикл идет по массиву аргв и проверяет  в
                                    // каком индексе есть флаг
    //( исключая его) - он переписывает все в массив
    if (argv[i][0] != '-')  // если текущий аргумент не является флагом
    {
      pos_args[count] = argv[i];  // записываем аргумент
      ++count;  // увеличиваем кол-во позиционных аргументов (фактически)
    }
  }

  return count;  // возвращает счетчиком - сколько файлов найдено
}

int main(int argc, char *argv[]) {
  flags cats;         // обьявил структуру со флагами
  init_flags(&cats);  // инициализируем все флаги нулями

  // printf("argc = %d\n", argc);
  if (argc > 1) {  // если переданы аргументы
    handle_flags(&cats, argc, argv);  // проверка  какие флаги присутсвуют

      char *pos_args[argc - 1];  // массив позиционных аргументов (длинна макс)
      /*позиционные аргументы это те  которые не сдвигаются
      cats test1.txt -n test2.txt
      cats -n test1.txt test2.txt
      */
      cats.files = get_pos_args(pos_args, argc, argv);

      // кол-во позиционных аргументов (фактически сколько записано) (путей к
      // файлам)

      /*pos_args - массив содержащий по очереди пути к файлам
       cats.files - количество этих  файлов (таким образом мы исключаем флаг из
       очереди для перебора аргументов где бы он в массиве argv  не находился)
       cats - структура которая на данный момент времен и содержит флаг который
       активен
       */
    if (cats.none) {  // если среди аргументов есть флаг

      if (cats.files != 0) {  // если есть сами файлы cat + flag + file
        cat_with_files(&cats, cats.files, pos_args);
        // вывод потоков в stdout  с ообработкой аргументов
      } else { // если файлов нет cat + flag
        FILE *streams[1] = {stdin};
        /* так как output_streams - принимает массив потоков - делаем массив на
         * 1 поток stdin*/
        output_streams(&cats, streams, 1);
      }
    }else{ // cat + file
        cat_with_files(&cats, cats.files, pos_args);
    }

  } else { // cat (без флагов)
    FILE *streams[1] = {stdin};
    /* так как output_streams - принимает массив потоков - делаем массив на 1
     * поток stdin*/
    output_streams(&cats, streams, 1);
  }

  return 0;
}

/*   общая логика программы:
   -------------------------------------------------------------------------------------------------
   Есть хоть какие-то аргументы (флаги / файлы ):
   -------------------------------------------------------------------------------------------------
   Проверка какие флаги присутсвуют:
   Записывается в отдельную структру какие из списка есть -  какие флаги, есть ли какой-то.
   
   Дублируются имена файлов во временный массив из массива переданных аргументов
   - исклюая флаг как аргумент, и кол-во файлов(котоыре обуславливают массив  записывается в структуру)).

   0.1 - Создается отдельный массив потоков открытых на чтение (то есть всех файлов
   которые были переданы) - и этот массив передается в функцию обработки
   вывода. 
   
   Где: 1 - если есть флаги  и файлы: прооисходит 0.1, в структуре флагов - создаются под флаги
   для корректной работы определенных функций, и осуществляются их алгоритмы.
        2 - только флаги: просто выводятся символы из потока stdin - применяя -n.
        3 - есть только файлы: прооисходит 0.1, Создается отдельный массив потоков открытых на чтение (то есть всех файлов
   которые были переданы) - и этот массив передается в функцию обработки
   вывода. 
    -------------------------------------------------------------------------------------------------
   Нет флагов и  (как аргумент к программе):
   -------------------------------------------------------------------------------------------------
    создается массив на 1 поток (stdin) и передается в функцию вывода.
*/