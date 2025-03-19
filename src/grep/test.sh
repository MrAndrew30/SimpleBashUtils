FLAGS=(-i -v -c -l -n -h -s)
FLAGS1=(i v c l n h s)
TESTS=(test0.txt test1.txt test2.txt)
FILES="../../data-samples/grep/test0.txt ../../data-samples/grep/test1.txt ../../data-samples/grep/test2.txt"
number_of_test=0
right_answers=0

compare(){
    number_of_test=$((number_of_test+1))
    ./s21_grep -e r$ -e ^a $flag $FILES > s21_result
    grep -e r$ -e ^a $flag $FILES > grep_result
    echo "Passing test with number "$number_of_test" flag $flag"
    if [[ $(diff -q grep_result s21_result) ]]
    then
        echo "-----------------------------"
        echo "grep -e r -e ^R $flag $FILES"
        echo "-----------------------------"
    else
        right_answers=$((right_answers+1))
        echo "$(diff -s grep_result s21_result)"
        echo
    fi
}

for flag1 in ${FLAGS[@]}
do
    for flag2 in ${FLAGS[@]}
    do  
        for flag3 in ${FLAGS[@]}
        do
            if [ $flag1 != $flag2 ] && [ $flag1 != $flag3 ] && [ $flag2 != $flag3 ]
            then
                flag="$flag1 $flag2 $flag3 "
                compare
            fi
        done
    done
done

for flag1 in ${FLAGS1[@]}
do
    for flag2 in ${FLAGS1[@]}
    do  
        for flag3 in ${FLAGS1[@]}
        do
            if [ $flag1 != $flag2 ] && [ $flag1 != $flag3 ] && [ $flag2 != $flag3 ]
            then
                flag="-$flag1$flag2$flag3 "
                compare
            fi
        done
    done
done

./s21_grep r $FILES > s21_result
grep r $FILES > grep_result
number_of_test=$((number_of_test+1))
if [[ $(diff -q grep_result s21_result) ]]
then
    echo "-----------------------------"
    echo "Failed extra test"
    echo "-----------------------------"
else
    right_answers=$((right_answers+1))
    echo "$(diff -s grep_result s21_result)"
    echo
fi

echo
echo "-----------------------------"
echo "Correct answers:"
echo $right_answers / $number_of_test

rm -rf s21_result grep_result