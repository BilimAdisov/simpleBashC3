CFLGS=-Wall -Wextra -Werror -std=c11

all: s21_grep

s21_grep: s21_grep.o
	gcc $(CFLGS) s21_grep.o -o s21_grep

s21_grep.o: s21_grep.c s21_grep.h
	gcc $(CFLGS) -c s21_grep.c -o s21_grep.o

test: 
	sh test_grep.sh

style:
	clang-format -i s21_grep.c

style_check:
	clang-format -n s21_grep.c

clean:
	rm -rf *.o

rebuild: clean all
