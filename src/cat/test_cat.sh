#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
"VAR test_cat.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_cat $t > test_s21_cat.log
    cat $t > test_cat.log
    DIFF_RES="$(diff -s test_s21_cat.log test_cat.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_cat.log and test_cat.log are identical" ]
    then
      (( SUCCESS++ ))
        echo "$FAIL $SUCCESS $t"
    else
      (( FAIL++ ))
        echo "$FAIL $SUCCESS $t"
    fi
    rm test_s21_cat.log test_cat.log
}

# 1 параметр
for var1 in b e n s t v
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 2 параметра
for var1 in b e n s t v 
do
    for var2 in b e n s t v
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

# 3 параметра
for var1 in b e n s t v 
do
    for var2 in b e n s t v
    do
        for var3 in b e n s t v
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done

# 4 параметра
for var1 in b e n s t v 
do
    for var2 in b e n s t v
    do
        for var3 in b e n s t v
        do
            for var4 in b e n s t v
            do
                if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ] && [ $var1 != $var4 ] && [ $var2 != $var4 ] && [ $var3 != $var4 ]
                then
                    for i in "${tests[@]}"
                    do
                        var="-$var1 -$var2 -$var3 -$var4"
                        testing $i
                    done
                fi
            done
        done
    done
done

echo "FAIL: $FAIL"
echo "SUCCESS: $SUCCESS"
echo "ALL: $COUNTER"
