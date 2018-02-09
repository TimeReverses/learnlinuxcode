#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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
		//睡眠1秒
		sleep(1);
		printf("parent\n");
	}
	else
	{
		//使用execlp执行一个进程,NULL是结束标记(哨兵)
		execlp("ls","ls","-la",NULL);
	}
	return 0;
}
