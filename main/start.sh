#!/bin/bash
# set n to 1
n=1
d=0 
# continue until $n equals 5
while [ $n -le 5 ]
do
	./server
	myShellVar=$?
	echo "$myShellVar"
	echo "Welcome $d times."
done
