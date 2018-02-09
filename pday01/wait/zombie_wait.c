#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid;
	//	pid_t wpid;
	pid = fork();
	printf("pid = %u\n",pid);
	if(pid == 0)
	{
		printf("i am child process and my parent's pid is %u\n",getppid());
		sleep(10);
		printf("child process will die\n");	
	}
	else if(pid >= 0)
	{
		pid_t wpid = wait(NULL);
		//wait的功能:阻塞等待子进程退出,回收子进程残留资源,查看子进程的结束状态
		if(wpid == -1)
		{
			perror("wait error");
			exit(1);
		}
		printf("i am parent process and my pid is %u\n",getpid());
	}
	else
	{
		perror("fork");
		exit(1);
	}
	return 0;
}
