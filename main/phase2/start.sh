#!/bin/ash
trap "exit 0" 2
while true
do
    ./server &
done
