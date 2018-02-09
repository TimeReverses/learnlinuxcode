#! /bin/sh

time=5 #5次机会
echo "please input your password:"
read passwd
while [ "$passwd" != "nihao" ]
do
	if [ "$time" -eq 1 ]; then  #如果小于等于0,退出
		break 1
	fi
	time=$[$time - 1];
	echo "your password is wrong,you left $time times."
	read passwd
done
