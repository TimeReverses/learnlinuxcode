#! /bin/bash

read NUM
if [ "$NUM" -gt 10 ]; then
	echo "$NUM greater than 10"
elif [ "$NUM" -lt -10 ]; then
	echo "$NUM less than -10"
else
	echo "$NUM between -10 and 10"
fi
