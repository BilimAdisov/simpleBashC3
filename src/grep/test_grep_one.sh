#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF=""

gcc -Werror -Wextra -Wall s21_grep.c -o s21_grep

all_flags=("-e" "-i" "-v" "-c" "-l" "-n" "-h" "-o" )
bonus_flag=("-i" "-v" "-c" "-l" "-n" "-h" )
f_e_flag=("-e" "-f")
fflag=("-f")
f_pat=("test/pattern_1.txt" "test/pattern_2.txt")
patterns=("Depp" "english")
input_files=("test/test_1.txt" "test/test_2.txt")



echo ""
echo "###################"
echo "#### ONE FILES ####"
echo "###################"
echo ""
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
echo ""
echo "###################"
echo "#### TWO FILES ####"
echo "###################"
echo ""
# Цикл по всем паттернам и файлам ввода
for flags in "${all_flags[@]}"
do
    for pattern in "${patterns[@]}"
    do
        for file_1 in "${input_files[@]}"
        do
            for file_2 in "${input_files[@]}"
            do
                if [ $file_1 != $file_2 ]
                then
                    echo "$flags $pattern $file_1 $file_2:"
                    # Выполнение вашей программы
                    ./s21_grep "$flags" "$pattern" "$file_1" "$file_2"> your_output.txt
                    # Выполнение оригинального grep
                    grep "$flags" "$pattern" "$file_1" "$file_2" > original_output.txt
                    
                    # Сравнение результатов
                    if diff -q your_output.txt original_output.txt >/dev/null 2>&1;
                    then
                        let "COUNTER++"
                        let "SUCCESS++"
                        echo "\033[92m$COUNTER\033[0m/ - Success"
                    else 
                        let "COUNTER++"
                        let "FAIL++"
                        echo "\033[91m$COUNTER\033[0m/ - Fail"
                    fi

                    # Удаление временных файлов
                    rm your_output.txt
                    rm original_output.txt
                fi
            done
        done
    done
done

echo ""
echo "###################"
echo "#### TWO FLAGS ####"
echo "###################"
echo ""

for flag_1 in "${bonus_flag[@]}"
do
    for flag_2 in "${bonus_flag[@]}"
    do
        for pattern in "${patterns[@]}"
        do
            for input_file in "${input_files[@]}"
            do 
                if [ $flag_1 != $flag_2 ]
                then
                    echo "$flag_1 $pattern $flag_2  $input_file :"
                    # Выполнение вашей программы
                    ./s21_grep "$flag_1" "$pattern" "$flag_2"  "$input_file" > your_output.txt
                    # Выполнение оригинального grep
                    grep "$flag_1" "$pattern" "$flag_2"  "$input_file" > original_output.txt
                    
                    # Сравнение результатов
                    if diff -q your_output.txt original_output.txt >/dev/null 2>&1;
                    then
                        let "COUNTER++"
                        let "SUCCESS++"
                        echo "\033[92m$COUNTER\033[0m/ - Success"
                    else 
                        let "COUNTER++"
                        let "FAIL++"
                        echo "\033[91m$COUNTER\033[0m/ - Fail"
                    fi

                    # Удаление временных файлов
                    rm your_output.txt
                    rm original_output.txt
                fi
            done
        done
    done
done

echo ""
echo "###################"
echo "## (E OF F) FLAG ##"
echo "###################"
echo ""


for flag in "${f_e_flag[@]}"
do
    for pattern_1 in "${patterns[0]}"
    do
        for pattern_2 in "${patterns[1]}"
        do
            for input_file in "${input_files[@]}"
            do 
                echo "$flag $pattern_1 $flag $pattern_2 $input_file :"
                # Выполнение вашей программы
                ./s21_grep "$flag" "$pattern_1" "$flag" "$pattern_2" "$input_file" > your_output.txt
                # Выполнение оригинального grep
                grep "$flag" "$pattern_1" "$flag" "$pattern_2"  "$input_file" > original_output.txt
                
                # Сравнение результатов
                if diff -q your_output.txt original_output.txt >/dev/null 2>&1;
                then
                    let "COUNTER++"
                    let "SUCCESS++"
                    echo "\033[92m$COUNTER\033[0m/ - Success"
                else 
                    let "COUNTER++"
                    let "FAIL++"
                    echo "\033[91m$COUNTER\033[0m/ - Fail"
                fi

                # Удаление временных файлов
                rm your_output.txt
                rm original_output.txt
            done
        done
    done
done


echo ""
echo "###################"
echo "##### F  FLAG #####"
echo "###################"
echo ""


for flag in "${fflag[@]}"
do
    for pattern_1 in "${f_pat[0]}"
    do
        for pattern_2 in "${f_pat[1]}"
        do
            for input_file in "${input_files[@]}"
            do 
                echo "$flag $pattern_1 $flag $pattern_2 $input_file :"
                # Выполнение вашей программы
                ./s21_grep "$flag" "$pattern_1" "$flag" "$pattern_2" "$input_file" > your_output.txt
                # Выполнение оригинального grep
                grep "$flag" "$pattern_1" "$flag" "$pattern_2"  "$input_file" > original_output.txt
                
                # Сравнение результатов
                if diff -q your_output.txt original_output.txt >/dev/null 2>&1;
                then
                    let "COUNTER++"
                    let "SUCCESS++"
                    echo "\033[92m$COUNTER\033[0m/ - Success"
                else 
                    let "COUNTER++"
                    let "FAIL++"
                    echo "\033[91m$COUNTER\033[0m/ - Fail"
                fi

                # Удаление временных файлов
                rm your_output.txt
                rm original_output.txt
            done
        done
    done
done

echo ""
echo "###################"
echo "##### THE END #####"
echo "###################"
echo ""
echo "Total: $COUNTER, Success: $SUCCESS, Fail: $FAIL"
echo ""