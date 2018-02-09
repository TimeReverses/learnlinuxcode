#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
	int fd[2];
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
	{//子进程 读
		//关闭管道的写文件操作符
		close(fd[1]);
		//将标准输入重定向给管道读文件描述符
		dup2(fd[0],STDIN_FILENO);
		execlp("wc","wc","-l",NULL);
		/*
		//读取管道中的数据
		char buf[1024] = {0};
		ret = read(fd[0],buf,sizeof(buf));
		char* argv[1024];//参数列表
		argv[0] = "wc";
		argv[1] = "-l";
		int i = 2;
		char* temp;
		if(ret > 0)
		{//读取到数据
			while((temp = strtok(buf,"\n")) != NULL)
			{
				argv[i] = temp;
				i++;
			}
			argv[i] = NULL;
			execvp("wc",argv);
			//printf("buf = %s\n",buf);
		}	
		*/
	}
	if(pid > 0)
	{//父进程 写
		//关闭管道的读文件操作符
		close(fd[0]);
		//重定向标准输出
		dup2(fd[1],STDOUT_FILENO);
		//写数据
		execlp("ls","ls",NULL);
		//回收子进程
		//wait(NULL);
	}
	return 0;
}
