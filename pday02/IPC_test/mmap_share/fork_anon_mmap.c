#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*
linux下使用匿名映射区的方法
使用MAP_ANONYMOUS  或 MAP_ANON宏
*/
int var = 100;
int main(void)
{
	pid_t pid;
	int *p = NULL;
	//创建一个映射区,大小不限,指定fd为-1
	p = mmap(NULL,4,PROT_READ | PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
	if(p == MAP_FAILED)
	{
		perror("mmap error");
		exit(1);
	}
	//创建子进程
	pid = fork();
	if(pid == -1)
	{
		perror("fork error");
		exit(1);
	}
	if(pid == 0)
	{
		//子进程
		*p = 1000;
		var = 2000;
		printf("*p = %d,var = %d\n",*p,var);
	}
	if(pid > 0)
	{
		sleep(1);
		printf("*p = %d,var = %d\n",*p,var);
		wait(NULL);
	}
	int ret = munmap(p,4);
	if(ret == -1)
	{
		perror("munmap error");
		exit(1);
	}
	return 0;
}
