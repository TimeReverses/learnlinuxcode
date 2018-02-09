#! /bin/sh

TIME=0 #次数
echo "please input your passwd."
read PASSWD
while [ "$PASSWD" != "hello" -a $TIME -lt 3 ]; do
	TIME=$[$TIME+1]
	echo "sorry,your password is error.please try again! $TIME times."
	read PASSWD
done
