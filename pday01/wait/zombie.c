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
	else if(pid == 0)
	{
		printf("i am child process and my parent id is %u\n",getppid());
		sleep(10);
		printf("10 seconds later,child process will die\n");
	}
	else
	{
		while(1)
		{
			printf("i am parent process and my pid is %u\n",getpid());
			sleep(1);
		}
	}
	return 0;
}
