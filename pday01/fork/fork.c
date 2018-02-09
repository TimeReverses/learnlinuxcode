#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main(void)
{
	printf("xxxxxxxxxxxxxxx\n");
	int ret = fork();
	if(ret == -1)
	{
		perror("fork");
		exit(1);
	}
	if(ret > 0)
	{
		printf("i am parent my pid = %u,ppid = %u\n",getpid(),getppid());
		//休眠1秒
		sleep(1);
	}
	if(ret == 0)
	{
		printf("i am child my pid = %u,ppid = %u\n",getpid(),getppid());
	}


	printf("YYYYYYYYYYYYYYY\n");
	return 0;
}
