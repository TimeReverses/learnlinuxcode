#! /bin/sh

is_directory()
{
	DIR_NAME=$1
	echo "$DIR_NAME"
	if [ ! -d "$DIR_NAME" ]; then
		return 1
	else
		return 0
	fi
}

for DIR in "$@"; do
	#给函数传递值
	if is_directory "$DIR"
	then
		echo "$DIR has exist."
	else
		echo "$DIR doesn't exist.Creating it now..."
		mkdir $DIR > /dev/null 2>&1
		if [ $? -ne 0 ]; then
			echo "Cannot create dirctory $DIR"
			return 1;
		fi
	fi
done
