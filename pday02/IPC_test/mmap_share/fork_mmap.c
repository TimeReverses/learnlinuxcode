#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

//全局变量
int var = 100;
int main(void)
{
	//创建一个临时的文件
	int fd = open("temp",O_CREAT | O_RDWR | O_TRUNC,0644);
	if(fd == -1)
	{
		perror("file open");
		exit(1);
	}
	//拓展空间
	int ret = ftruncate(fd,4);
	if(ret == -1)
	{
		perror("ftruncate error");
		exit(1);
	}
	//创建一个映射区
	int *p = NULL;
	//p = mmap(NULL,4,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	p = mmap(NULL,4,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);
	if(p == MAP_FAILED)
	{
		perror("mmap error");
		exit(1);
	}
	//关闭文件
	close(fd);
	//unlink
	unlink("temp");//删除文件的目录项,成为系统回收文件的条件
	pid_t pid;//进程id
	pid = fork();
	if(pid == -1)
	{
		perror("fork error");
		exit(1);
	}
	if(pid == 0)
	{//子进程
		*p = 1000;
		var = 2000;
		printf("*p = %d,var = %d\n",*p,var);
		//释放映射区
		//munmap(p,4);
	}
	if(pid > 0)
	{
		sleep(1);//休眠1秒
		printf("*p = %d,var = %d\n",*p,var);
		wait(NULL);
		//释放映射区
		int ret = munmap(p,4);
		if(ret == -1)
			perror("munmap error");
	}
	return 0;
}
