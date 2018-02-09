#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

int main(void)
{
	//获取当前进程id
/*
	pid_t pid = getpid();
	int ret = kill(pid,SIGKILL);//写宏名
	if(ret == -1)	
	{
		perror("kill -9");
		exit(1);
	}
*/
	//创建5个子进程
	int n = 5;
	int i;
	pid_t subpid,pid;
	for(i = 0;i< n;i++)
	{
		pid = fork();
		if(pid == -1)
		{
			perror("fork error");
			exit(1);
		}
		if(pid == 0)
		{
			if(i == 2)
			{
				subpid = getpid();
				sleep(3);
			}
			break;
		}
	}
	if(i == n)
	{
		//父进程
		sleep(1);
		kill(subpid,SIGKILL);//杀死子进程
		//回收子进程
		do
		{
			waitpid(-1,NULL,WNOHANG);	
			n--;
		}while(n>1);
	}
	
	return 0;
}
