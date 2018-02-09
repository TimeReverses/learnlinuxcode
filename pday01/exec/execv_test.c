#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
	{
		sleep(1);
		printf("parent\n");
	}
	else
	{
		//使用execv,执行一个程序
		//首先需要参数列表数组
		char* argv[] = {"ls","-l","-a",NULL};
		execv("/bin/ls",argv);
	}
	return 0;
}
