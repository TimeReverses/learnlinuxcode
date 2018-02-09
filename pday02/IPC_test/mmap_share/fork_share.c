#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	//文件描述符
	int fd1,fd2;
	//测试文本
	char* str = "hello linux,this is a test program.\n";
	//创建一个子进程
	char buf[512] = {0};
	pid_t pid;
	pid = fork();
	if(pid == -1)
	{
		perror("fork error");
		exit(1);
	}
	if(pid == 0)
	{
		//子进程	
		//打开文件,并将测试文本写入到文件中,并关闭文件
		fd1 =open("test.txt",O_RDWR);
		if(fd1 == -1)
		{
			perror("file open");
			exit(1);
		}
		write(fd1,str,strlen(str));
		close(fd1);
	}		
	if(pid > 0)
	{
		//父进程中,首先打开文件,然后休眠1秒钟,为了是子进程可以写入数据,在读文件
		fd2 = open("test.txt",O_RDWR);
		if(fd2 == -1)
		{
			perror("file open");
			exit(1);
		}
		sleep(1);
		int len = read(fd2,buf,sizeof(buf));
		//将读出的数据写入到屏幕上
		write(STDOUT_FILENO,buf,len);
		//关闭文件
		close(fd2);
	}
	return 0;
}
