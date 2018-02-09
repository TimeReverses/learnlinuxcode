#! /bin/bash

if [ -d test1 ]; then
	echo "it is a dir"
elif [ -f test1 ]; then
	echo "it is a file"
else
	printf "unknow\n"
fi
