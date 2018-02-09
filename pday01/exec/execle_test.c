#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char** environ;

int main(void)	
{
	pid_t pid = fork();
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
		execle("/bin/ls","ls","-l","-a",NULL,environ);
	}
	return 0;
}
