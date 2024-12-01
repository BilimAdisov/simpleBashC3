#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
"s test_0.txt VAR"
"int test_4.txt VAR"
"for s21_grep.c s21_grep.h Makefile VAR"
"for s21_grep.c VAR"
"-e for -e ^int s21_grep.c s21_grep.h Makefile VAR"
"-e for -e ^int s21_grep.c VAR"
"-e regex -e ^print s21_grep.c VAR -f test_pattern.txt"
"-e while -e void s21_grep.c Makefile VAR -f test_pattern.txt"
)

declare -a extra=(
"-n for test_1.txt test_2.txt"
"-n for test_1.txt"
"-ce ^int test_1.txt test_2.txt"
"-e ^int test_1.txt"
"-nivh = test_1.txt test_2.txt"
"-ie INT test_5.txt"
"-echar test_1.txt test_2.txt"
"-ne = -e out test_5.txt"
"-iv int test_5.txt"
"-in int test_5.txt"
"-c -l aboba test_1.txt test_5.txt"
"-v test_1.txt -e ank"
"-ne ) test_5.txt"
"-l for test_1.txt test_2.txt"
"-e = -e out test_5.txt"
"-ne ing -e as -e the -e not -e is test_6.txt"
"-e ing -e as -e the -e not -e is test_6.txt"
"-c -e . test_1.txt -e '.'"
"-l for no_file.txt test_2.txt"
"-f test_3.txt test_5.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep $t > test_s21_grep.log
    grep $t > test_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_grep.log are identical" ]
    then
      (( SUCCESS++ ))
      echo "$FAIL $SUCCESS $t"
    else
      (( FAIL++ ))
      echo "$FAIL $SUCCESS $t"
    fi
    rm test_s21_grep.log test_grep.log
}

# специфические тесты
for i in "${extra[@]}"
do
    testing $i
done

# 1 параметр
for var1 in v c l n h 
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 2 параметра
for var1 in v c l n h 
do
    for var2 in v c l n h 
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done


# 2 сдвоенных параметра
for var1 in v c l n h 
do
    for var2 in v c l n h 
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                testing $i
            done
        fi
    done
done

echo "FAIL: $FAIL"
echo "SUCCESS: $SUCCESS"
echo "ALL: $COUNTER"
