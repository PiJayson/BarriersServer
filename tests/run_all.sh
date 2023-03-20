#!/bin/sh

. common

TEST_DIR=tests


BARRIER_ID=1
$BIN/init_barrier.x $BARRIER_ID 2 || { echo "Simple barrier initialization failed." ; exit 1; }
$BIN/destroy_barrier.x $BARRIER_ID || { echo "Simple barrier destruction failed." ; exit 1; }

# echo "Running tests:"

for t in $(ls $TEST_DIR)
do
	./run_test.sh $TEST_DIR/$t
done
