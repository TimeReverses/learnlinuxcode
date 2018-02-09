#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
	//使用只读的方式打开FIFO文件
	int fd = open(argv[1],O_RDONLY);
	if(fd == -1)
	{
		perror("open file");
		exit(1);
	}
	
	//循环读取数据
	char buf[1024] = {0};
	while(1)
	{
		int len = read(fd,buf,sizeof(buf));
		//将读取到的数据打印到屏幕上
		write(STDOUT_FILENO,buf,len);
		sleep(1);
	}
	//关闭文件
	close(fd);
	return 0;
}
