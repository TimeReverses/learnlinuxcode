#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*
类UNIX下使用匿名映射区的方法
不能使用MAP_ANONYMOUS  或 MAP_ANON宏
只能打开系统提供的设备文件   "/dev/zero"   或  "/dev/null"文件
*/
int var = 100;
int main(void)
{
	pid_t pid;
	int *p = NULL;
	int fd = open("/dev/zero",O_RDWR);
	if(fd == -1)
	{
		perror("file open");
		exit(1);
	}
	//创建一个映射区,大小不限
	p = mmap(NULL,4,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
	if(p == MAP_FAILED)
	{
		perror("mmap error");
		exit(1);
	}
	//关闭文件
	close(fd);
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
