#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	//创建一个文件描述符变量
	int fd;
	//打开文件
	fd = open("nongfu.txt",O_RDONLY);
	//判断文件描述符是否正确
	if(fd == -1)
	{
		perror("file open");
		exit(1);
	}
	//创建一个新文件,用于复制的目标文件
	int fd2;
	fd2 = open("nongfu",O_RDWR | O_CREAT,0664);
	if(fd2 == -1)
	{
		perror("file open");
		//关闭上面的文件描述符
		int ret = close(fd);
		if(ret == -1)
		{
			perror("file close");
		}
		exit(1);
	}	
	//创建一个缓冲区数组,并初始化为0
	char tmp[1024*8] = {0};
	//传输字符串的个数变量
	int count;
	//写入成功的字符的数量
	int num;
	//两个文件都打开成功了,开始复制操作
	while((count = read(fd,tmp,sizeof(tmp))) != -1)
	{
		//读取到的字符数量为0,说明已经到文件尾了
		if(count == 0)
		{
			break;
		}
		//开始写到目标文件中
		num = write(fd2,tmp,count);
		//判断写入到文件中是否有异常
		if(num == -1)
		{
			perror("write file");
			break;
		}
		//判断是否写入的字符和读出的字符数量相等
		if(num == count)
		{
			printf("写入成功%d个字节\n",num);
		}
	}
	//关闭文件
	int ret = close(fd);
	if(ret == -1)
	{
		perror("close read file");
	}
	ret = close(fd2);
	if(ret == -1)
	{
		perror("close write file");
		exit(1);
	}
	return 0;
}
