#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int i=0;
	pid_t pid;
	for(i = 0;i<5;i++)
	{
		pid = fork();
		//判断fork返回的id值
		if(pid == -1)
		{
			perror("fork");
			exit(1);
		}
		//判断是否是子进程调用的fork返回
		if(pid == 0)
		{
			break;
		}
	}
	if(i == 5)
	{
		sleep(i);
		printf("i am parent process and my pid is %u\n",getpid());
	}
	else
	{
		sleep(i);
		printf("i am %dth child process and my pid is %u\n",i+1,getpid());
	}
	return 0;
}
