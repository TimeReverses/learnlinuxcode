#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	//首先将文件打开
	int fd = open("tempfile",O_CREAT | O_RDWR,0644);
	if(fd == -1)
	{
		perror("file open");
		exit(1);
	}
	//删除临时文件
	int ret = unlink("tempfile");
	if(ret == -1)
	{
		perror("unlink");
	}
	//write file
	write(fd,"hello\n",6);

	//重置文件的指针
	lseek(fd,0,SEEK_SET);
	//read file
	char buf[64] = {0};
	int len = read(fd,buf,sizeof(buf));

	//将读出的数据写到屏幕上
	write(1,buf,len);
	//关闭文件
	ret = close(fd);
	if(ret == -1)
	{
		perror("file close");
		exit(1);
	}	
	return 0;
}
