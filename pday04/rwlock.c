#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//读写锁
pthread_rwlock_t rwlock;
int publicvar = 100;//共享资源

void* th_write(void* arg)//写锁线程的主调函数
{
	while(1)
	{
		int i = (int)arg;
		//加写锁,写锁独占
		pthread_rwlock_wrlock(&rwlock);
		printf("--------write lock : %d,thread id : %lu,public var: %d\n",i,pthread_self(),++publicvar);
		pthread_rwlock_unlock(&rwlock);
		usleep(100);
	}
	pthread_exit((void*)1);
}

void* th_read(void* arg)//读锁线程的主调函数
{
	while(1)
	{
		int i = (int)arg;
		//加读锁,读锁共享,写锁优先级高
		pthread_rwlock_wrlock(&rwlock);
		printf("----------------------read lock : %d,public var: %d\n",i,publicvar);
		pthread_rwlock_unlock(&rwlock);
		usleep(100);
	}
	pthread_exit((void*)1);
}

int main(void)
{
	//线程的id数组
	pthread_t tid[8];//8个线程,4个读线程,4个写线程
	//初始化读写锁
	pthread_rwlock_init(&rwlock,NULL);
	//创建线程
	for(int i=0;i<4;i++)
		pthread_create(&tid[i],NULL,th_write,(void*)i);
	for(int i=0;i<4;i++)
		pthread_create(&tid[i + 4],NULL,th_read,(void*)i);
	//回收子线程
	for(int i=0;i<8;i++)
		pthread_join(tid[i],NULL);
	//销毁锁
	pthread_rwlock_destroy(&rwlock);
	return 0;
}
