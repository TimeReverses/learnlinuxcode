#! /bin/sh

for filename in $(ls); do
	if [ -d "$filename" ]; then
		echo "$filename is a dir."
	elif [ -f "$filename" ]; then
		echo "$filename is a file."
	else
		echo "$filename is other file type."
	fi
done
