#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	//首先打开一个文件
	int fd = open("a.txt",O_RDWR);
	if(fd == -1)
	{
		perror("open file");
		exit(1);
	}
	printf("current file fd : %d\n",fd);
	//复制文件描述符
	int ret = dup(fd);
	if(ret == -1)
	{
		perror("dup file");
		exit(1);
	}
	printf("dup fd : %d\n",ret);
	//使用oldfd和newfd分别操作文件
	char buf1[128] = "你好这是一个真实的世界,没有任何东西,只有欲望\n";
	char buf2[128] = "hello new world,welcome to new world,we can share everything in here\n";
	//将字符串写入到文件中
	write(fd,buf1,strlen(buf1));
	write(ret,buf2,strlen(buf2));
	//关闭文件
	close(fd);
	write(ret,"hello linux\n",12);
	close(ret);
	return 0;	
}

