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
		printf("parent \n");
	}
	else
	{
		//子进程的执行程序,使用execl需要程序的路径
		//execl("/bin/ls","ls","-l","-a",NULL);
		//执行自己的程序
		execl("./while","while",NULL);
	}
	return 0;
}
