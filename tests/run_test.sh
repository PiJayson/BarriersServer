#!/bin/sh

. common

trap '' 2

if [ $# != 1 ]; then 
	echo Syntax: $0 test_script_name;
	exit 1;
fi

BASE_DIR=$PWD

TEST_PATH=$(readlink -f $1)
if [ -z  $TEST_PATH ]; then 
	echo Test script \" $1 \" not found
	exit 1
fi 
TEST_NAME=$(basename $TEST_PATH)

echo "Test " $TEST_NAME ": " 

outf=$OUTPUT_DIR/$TEST_NAME.out
errf=$OUTPUT_DIR/$TEST_NAME.err

echo $TEST_PATH ">" $outf "2>"$errf
$TEST_PATH 3>&0 > $outf 2> $errf || { echo "Test failed - exited with $?."; exit 2; }

if [ -f $CHECK_DIR/$TEST_NAME ]; then
	echo $CHECK_DIR/$TEST_NAME "<" $outf ">" $outf.tmp
	$CHECK_DIR/$TEST_NAME < $outf > $outf.tmp
	mv $outf.tmp $outf	
fi


diff $outf $EXPECTED_DIR/$TEST_NAME.out > /dev/null
result1=$?

result2=0
if [ -f $EXPECTED_DIR/$TEST_NAME.err ]; then
	diff $errf $EXPECTED_DIR/$TEST_NAME.err > /dev/null
	result2=$?
fi

if [ $result1 -eq 0 ] && [ $result2 -eq 0 ]; 
	then echo OK; 
	else { echo FAIL ; exit 3; }
fi

exit 0


