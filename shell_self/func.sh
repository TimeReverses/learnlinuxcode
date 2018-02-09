#! /bin/sh

foo()
{
	echo "this is a function."
}

echo $#
echo $@
echo $0
echo $1
echo $2
echo $3

#函数调用
echo "----start------"
foo
echo "-----end-------"
