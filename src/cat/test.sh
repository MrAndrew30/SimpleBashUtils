FLAGS=(-b -e -n -s -t -v -A --number-nonblank --number --squeeze-blank -T)
TESTSPATH=../../data-samples/cat/
TESTS=(test0.txt bytes.txt spaces.txt)
FILES="../../data-samples/cat/test0.txt ../../data-samples/cat/bytes.txt ../../data-samples/cat/spaces.txt"
number_of_test=0
right_answers=0

compare(){
    number_of_test=$((number_of_test+1))
    ./s21_cat $flag $FILES > s21_result
    cat $flag $FILES > cat_result
    echo "Passing test with number "$number_of_test" flag $flag"
    if [[ $(diff -q cat_result s21_result) ]]
    then
        echo "$(diff -q cat_result s21_result) $flag $FILES"
    else
        right_answers=$((right_answers+1))
        echo "$(diff -s cat_result s21_result)"
        echo
    fi
}

for flag in ${FLAGS[@]}
do
    compare
done

echo
echo "-----------------------------"
echo "Correct answers:"
echo $right_answers / $number_of_test

rm -rf s21_result cat_result