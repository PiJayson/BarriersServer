#!/bin/sh

. common

CH=""
$BIN/init_barrier.x 7 3 > /dev/null
$BIN/wait.x 7 > /dev/null &
CH=${CH}" $!"
$BIN/wait.x 7 > /dev/null &
CH=${CH}" $!"
echo "barrier: 7 width: 3 queue:" $CH

CH=""
$BIN/init_barrier.x 94 4 > /dev/null
$BIN/wait.x 94 > /dev/null &
CH=${CH}" $!"
$BIN/wait.x 94 > /dev/null &
CH=${CH}" $!"
$BIN/wait.x 94 > /dev/null &
CH=${CH}" $!"
echo "barrier: 94 width: 4 queue:" $CH

CH=""
$BIN/init_barrier.x 17 5 > /dev/null
$BIN/wait.x 17 > /dev/null &
CH=${CH}" $!"
$BIN/wait.x 17 > /dev/null &
CH=${CH}" $!"
$BIN/wait.x 17 > /dev/null &
CH=${CH}" $!"
$BIN/wait.x 17 > /dev/null &
CH=${CH}" $!"
echo "barrier: 17 width: 5 queue:" $CH


sleep 2
cat /proc/barriers

$BIN/wait.x 7 > /dev/null &
$BIN/wait.x 94 > /dev/null &
$BIN/wait.x 17 > /dev/null &

wait

$BIN/destroy_barrier.x 7 > /dev/null 
$BIN/destroy_barrier.x 17 > /dev/null 
$BIN/destroy_barrier.x 94 > /dev/null 
