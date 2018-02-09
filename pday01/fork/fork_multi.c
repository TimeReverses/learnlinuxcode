#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
	int i =0;
	pid_t pid;
	printf("xxxxx\n");
	for(i = 0;i<5;i++)
	{
		//父进程一直创建子进程
		pid = fork();
		if(pid == -1)
		{
			perror("fork");
			exit(1);
		}
		if(pid == 0)
		{
			break;//子进程创建完成,直接退出循环
		}
	}	
	sleep(i);
	if(i < 5)
	{
		//sleep(i);
		printf("i am %dth child process and my pid is %u \n",i+1,getpid());
	}
	else
	{
		//sleep(i);
		printf("i am parent process and my pid is %u \n",getpid());
	}
	return 0;
}
