#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void)
{
	int fd[2];
	int n = 2;//子进程数
	pid_t pid,wpid;
	int i;
	//创建管道	
	int ret = pipe(fd);
	if(ret == -1)
	{
		perror("pipe error");
		exit(1);
	}
	//创建子进程
	for(i = 0;i < n;i++)
	{
		pid = fork();
		if(pid == -1)
		{
			perror("fork error");
			exit(1);
		}
		if(pid == 0)
		{//子进程
			break;
		}
	}
	//父进程
	if(i == n)
	{
		//首先关闭所有的管道读写文件描述符	
		close(fd[0]);
		close(fd[1]);
		do
		{
			//回收子进程的资源
			wpid = waitpid(-1,NULL,WNOHANG);
			if(wpid > 0)
			{//回收一个子进程
				n--;
			}
		}while(n > 0);
		printf("i am parent process and my pid is %u\n",getpid());
	}
	//第一个子进程,执行ls命令
	if(i== 0)
	{
		//写,关闭管道的读
		close(fd[0]);
		printf("i am %dth child process and my pid is %u my ppid is %u\n",i+1,getpid(),getppid());
		//重定向标准输出
		dup2(fd[1],STDOUT_FILENO);
		execlp("ls","ls",NULL);
	}
	//第二个子进程,执行wc -l 命令
	if(i == 1)	
	{
		//读,关闭管道的写
		close(fd[1]);
		printf("i am %dth child process and my pid is %u my ppid is %u\n",i+1,getpid(),getppid());
		//重定向标准输入
		dup2(fd[0],STDIN_FILENO);
		execlp("wc","wc","-l",NULL);
	}
}
