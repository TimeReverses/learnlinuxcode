#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
	//打开一个不存在的文件
	int fp;//文件描述符
	/*
	fp = open("hello",O_RDWR);//使用读写的方式打开
	//判断fp是否正确
	if(fp == -1)
	{
		perror("file open");
		exit(1);
	}
	
	//创建一个新的文件
	fp = open("hello",O_RDWR | O_CREAT,0644);//使用读写的方式打开
	//判断fp是否正确
	if(fp == -1)
	{
		perror("file open");
		exit(1);
	}
	
	//O_EXCL与O_CREAT联合使用判断改文件是否存在,如果存在,报错,不存在,创建一个文件
	fp = open("hello",O_RDWR | O_CREAT | O_EXCL,0644);//使用读写的方式打开
	//判断fp是否正确
	if(fp == -1)
	{
		perror("file open");
		exit(1);
	}
	*/
	//截断文件的内容,将文件设为0
	fp = open("hello",O_RDWR | O_TRUNC);//使用读写的方式打开
	//判断fp是否正确
	if(fp == -1)
	{
		perror("file open");
		exit(1);
	}
	
	printf("fp = %d\n",fp);
	//关闭文件
	int ret = close(fp);
	printf("ret = %d\n",ret);
	if(ret == -1)
	{
		perror("file close");
		exit(0);
	}
	return 0;
}
