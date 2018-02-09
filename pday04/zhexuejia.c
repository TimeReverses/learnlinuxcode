#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM 7
//共享资源
int sum = 100;
//锁
pthread_mutex_t mutex[NUM];

void* thread_func(void* arg)
{
	int i = (int)arg;//存储第几个线程
	pthread_mutex_t right;
	pthread_mutex_t left;
	while(1)
	{
		if(i == NUM -1)
		{
			//最后一个线程,比较特殊,不能再拿右手边的锁,而是拿左手边的锁
			right = mutex[0];
			left = mutex[i];
		}
		else
		{
			right = mutex[i];
			left = mutex[i + 1];
		}
		//首先获取右手边的锁
		pthread_mutex_lock(&right);	
		//在获取左手边的锁
		pthread_mutex_lock(&left);
		//将共享资源判断是否大于0,如果大于减1
		if(sum > 0)
		{
			printf("thread %d has eat one,left is %d\n",i+1,--sum);
		}
		else
		{
			pthread_exit(NULL);//退出
		}
		//释放锁
		pthread_mutex_unlock(&left);
		pthread_mutex_unlock(&right);
		sleep(rand() % 3);
	}
}

int main(void)
{
	srand(time(NULL));
	for(int i=0;i<NUM;i++)
	{
		//初始化锁
		pthread_mutex_init(&mutex[i],NULL);
	}
	pthread_t pid[NUM];
	//创建线程
	for(int i=0;i<NUM;i++)
	{
		pthread_create(&pid[i],NULL,thread_func,(void*)i);
	}
	//回收线程
	for(int i = 0;i<NUM;i++)
	{
		pthread_join(pid[i],NULL);
	}
	//销毁锁
	for(int i =0;i<NUM;i++)
	{
		pthread_mutex_destroy(&mutex[i]);
	}
	return 0;
}
