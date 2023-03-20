#!/bin/sh

bin/init_barrier.x 7 2
bin/wait.x 7 &
sleep 1
echo Stop server now. >&3
wait

