#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
	pid_t pid, wpid;
	int status;
	pid = fork();
	if(pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if(pid > 0)
	{
		wpid = wait(&status);
		if(WIFEXITED(status))
		{//正常退出
			printf("child exit with %d\n",WEXITSTATUS(status));
		}
		if(WIFSIGNALED(status))
		{
			printf("child killed by %d\n",WTERMSIG(status));
		}
		if(wpid == -1)
		{
			perror("wait error");
			exit(1);
		}
		printf("i am parent process\n");
	}

	if(pid == 0)
	{
		execl("./error","error",NULL);
		printf("i am child process\n");
		sleep(40);
		printf("child process will die\n");
	}
	return 0;
}
