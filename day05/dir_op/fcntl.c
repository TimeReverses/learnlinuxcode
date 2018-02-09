#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	//打开一个文件,使用只读的方式打开
	int fd = open("test.txt",O_WRONLY);
	if(fd == -1)
	{
		perror("open file");
		exit(1);
	}
	//测试用的字符串
	char *buf1 = "这是一个美好的世界,你看到的事物都是假的,是不是很美好.";
	char *buf2 = "社会主义好,这是真的吗?我不敢说,隔墙有耳,社会主义好啊,真的好";

	//首先,向文件中写入数据
	if(write(fd,buf1,strlen(buf1)) == -1)
	{
		perror("write");
		exit(1);
	}
	
	//获取文件状态的flag
	int flag = fcntl(fd,F_GETFL,0);
	if(flag == -1)
	{
		printf("fcntl getflag");
		exit(1);
	}
	//添加追加状态
	flag |= O_APPEND;
	
	//设置文件状态flag
	int ret = fcntl(fd,F_SETFL,flag);
	if(ret == -1)	
	{
		printf("fcntl setflag");
		exit(1);
	}
	//写入数据
	if(write(fd,buf2,strlen(buf2)) == -1)
	{
		perror("write");
		exit(1);
	}
	//关闭文件
	close(fd);
	return 0;	
}
