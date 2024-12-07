#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF=""

all_flags=("-e" "-i" "-v" "-c" "-l" "-n")
f_pat=("pattern_1.txt" "pattern_2.txt")
patterns=("Depp" "english")
input_files=("test_1.txt" "test_2.txt")

echo " "
echo " ONE FILES "
echo " "
for flags in "${all_flags[@]}"
do
    for pattern in "${patterns[@]}"
    do
        for input_file in "${input_files[@]}"
        do
            echo "$flags $pattern $input_file :"
            # Выполнение вашей программы
            ./s21_grep "$flags" "$pattern" "$input_file" > your_output.txt
            # Выполнение оригинального grep
            grep "$flags" "$pattern" "$input_file" > original_output.txt
            
            # Сравнение результатов
            if diff -q your_output.txt original_output.txt >/dev/null 2>&1;
            then
                let "COUNTER++"
                let "SUCCESS++"
                echo "\033[92m$COUNTER\033[0m - Success"
            else 
                let "COUNTER++"
                let "FAIL++"
                echo "\033[31m$FAIL\033[0m - Fail"
            fi

            # Удаление временных файлов
            rm your_output.txt
            rm original_output.txt
        done
    done
done


echo "Total: $COUNTER, Success: $SUCCESS, Fail: $FAIL"
