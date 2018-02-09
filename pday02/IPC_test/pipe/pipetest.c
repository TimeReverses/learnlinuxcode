#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void)
{
	int fd[2];//接受输出参数
	pid_t pid;
	//创建一个管道
	int ret = pipe(fd);
	if(ret == -1)
	{
		perror("pipe error");
		exit(1);
	}
	//创建一个子进程
	pid = fork();
	if(pid == -1)
	{
		perror("fork error");
		exit(1);
	}
	if(pid == 0)
	{//子进程  读取数据  关闭写的文件描述符
		//关闭管道中写的文件描述符
		close(fd[1]);
		char buf[512] = {0};
		//读取数据
		ret = read(fd[0],buf,sizeof(buf));
		//将数据打印到屏幕上
		if(ret > 0)
			write(STDOUT_FILENO,buf,ret);
	}
	if(pid > 0)
	{//父进程  写数据  关闭读的文件描述符
		//关闭管道中读数据的文件描述符
		close(fd[0]);
		sleep(1);
		//开始写数据
		write(fd[1],"hello pipeline\n",strlen("hello pipeline\n"));
		//回收子进程资源
		wait(NULL);
	}
	return 0;
}
