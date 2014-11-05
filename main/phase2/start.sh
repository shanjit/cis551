#!/bin/bash
trap "exit 0" 2
while true
do
    ./server_demo &
done
