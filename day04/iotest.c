#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int fd;
	fd = open("aa",O_RDWR);
	if(fd == -1)
	{
		perror("file open");
		exit;
	}
	//lseek(int fd,off_t offset,int whence);
	//使用lseek查看文件大小,获取文件长度
	int size = lseek(fd,0,SEEK_END);
	printf("size = %dbyte\n",size);
	//int humanSize = size / 1024;
	//printf("humanSize = %dkb\n",humanSize);
	//文件拓展
	int ret = lseek(fd,2000,SEEK_END);
	printf("return value %d\n",ret);

	//实现文件拓展,需要再做一次写操作
	write(fd,"a",1);
	//回到文件头
	size = lseek(fd,0,SEEK_SET);
	//关闭文件
	ret = close(fd);
	if(ret == -1)
	{
		perror("file close");
		exit(1);
	}	
	return 0;
}
