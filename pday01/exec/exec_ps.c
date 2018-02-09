#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	pid_t pid;
	pid = fork();
	if(pid == -1)
	{
		perror("fork");
		exit(1);
	}
	else if(pid > 0)
	{//父进程
		sleep(1);
		printf("parent");
	}
	else
	{//子进程
		//首先以读写的方式创建一个文件
		int fd = open("ps.txt",O_RDWR | O_CREAT,0644);
		if(fd == -1)
		{
			perror("file open");
			exit(1);
		}
		//STDOUT_FILENO是标准输出文件描述符的宏
		//将该文件的文件描述符复制给标准的输出流,使用dup2方法
		int ret = dup2(fd,STDOUT_FILENO);
		//int ret = dup(fd);
		printf("ret = %d\n",ret);
		execlp("ps","ps","aux",NULL);
		//关闭文件
		//close(fd);
		//close(ret);
	}	
	return 0;
}
