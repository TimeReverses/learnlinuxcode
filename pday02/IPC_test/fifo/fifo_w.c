#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int args,char* argv[])
{
	if(args < 2)
	{
		printf("./a.out fifoname\n");
		return -1;
	}
	//使用只写的方式打开FIFO文件
	int fd = open(argv[1],O_WRONLY);
	if(fd == -1)
	{
		perror("open file");
		exit(1);
	}
	
	//循环写入数据
	int i = 0;
	while(1)
	{
		char buf[128] = {0};
		sprintf(buf,"hello fifo %d\n",i++);
		write(fd,buf,strlen(buf));
		sleep(1);
	}
	//关闭文件
	close(fd);
	return 0;
}

