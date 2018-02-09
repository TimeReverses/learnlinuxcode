#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>

struct ms
{
	int num;
	pthread_mutex_t mutex;
	pthread_mutexattr_t mutexattr;
};

int main(void)
{
	pid_t pid;
	struct ms* mm;
	//创建一个mmap映射区,创建一个匿名的映射区
	mm = mmap(NULL,sizeof(struct ms),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);	
	if(mm == MAP_FAILED)
	{
		perror("mmap error");
		exit(1);
	}
	//初始化互斥属性变量
	pthread_mutexattr_init(&mm->mutexattr);
	pthread_mutexattr_setpshared(&mm->mutexattr,PTHREAD_PROCESS_SHARED);
	//初始化锁
	pthread_mutex_init(&mm->mutex,&mm->mutexattr);
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
		for(int i = 0;i<10;i++)
		{
			//上锁
			pthread_mutex_lock(&mm->mutex);
			//将映射区中的num数据加1
			(mm->num)++;
			printf("-----child process : %d\n",mm->num);
			pthread_mutex_unlock(&mm->mutex);
			//sleep(1);
		}
	}
	if(pid > 0)
	{
		//父进程
		for(int i = 0;i<10;i++)
		{
			//上锁
			pthread_mutex_lock(&mm->mutex);
			mm->num += 2;
			printf("------main process : %d\n",mm->num);
			pthread_mutex_unlock(&mm->mutex);
			//sleep(1);
		}
		wait(NULL);//回收子进程
		pthread_mutexattr_destroy(&mm->mutexattr);
		pthread_mutex_destroy(&mm->mutex);
	}
	return 0;
}
