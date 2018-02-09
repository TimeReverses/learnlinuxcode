#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	//打开一个文件
	int fd = open("english.txt",O_RDWR);
	if(fd == -1)
	{
		perror("open file1");
		exit(1);
	}
	printf("file fd : %d\n",fd);
	int fd2 = open("a.txt",O_RDWR);
	if(fd2 == -1)
	{
		perror("open file2");
		exit(1);
	}
	printf("file2 fd: %d\n",fd2);
	//将fd2的文件描述符赋值到fd上
	int ret = dup2(fd2,fd);
	if(ret == -1)
	{
		perror("dup2");
		exit(1);
	}
	printf("dup2 fd : %d \n",ret);
	//通过fd和fd2对文件进行操作
	char buf[128] = "never give up,never lose hope.always have \n";
	char buf2[128] = "never give up,never lose hope.always have \n";
	write(fd,buf,strlen(buf));
	write(fd2,buf2,strlen(buf2));
	//关闭文件
	close(fd);
	close(fd2);
	return 0;
}
