CFLGS=-Wall -Wextra -Werror -std=c11
all: s21_cat

s21_cat: s21_cat.o
	gcc $(CFLGS) s21_cat.o -o s21_cat

s21_cat.o: s21_cat.c s21_cat.h
	gcc $(CFLGS) -c s21_cat.c -o s21_cat.o

test: 
	sh t_cat.sh

style:
	clang-format -i s21_cat.c

style_check:
	clang-format -style=Google -n s21_cat.c

clean:
	rm s21_cat
	rm -rf *.o

rebuild: clean all