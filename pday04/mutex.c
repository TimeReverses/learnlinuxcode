#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
//在本程序中共享的资源是stdout

pthread_mutex_t mutex;
//新线程的主调函数
void* tfn(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		printf("hello ");
		sleep(rand() % 3);
		printf("world\n");
		//解锁
		pthread_mutex_unlock(&mutex);
		sleep(rand() % 3);
	}
	pthread_exit((void*)1);
}

int main(void)
{
	srand(time(NULL));
	pthread_t tid;
	//首先初始化一个锁
	pthread_mutex_init(&mutex,NULL); // mutex == 1

	//创建一个新线程
	pthread_create(&tid,NULL,tfn,NULL);

	while(1)
	{
		//加锁
		pthread_mutex_lock(&mutex); // mutex == 0
		printf("HELLO ");
		sleep(rand() % 3);
		printf("WORLD\n");
		//解锁
		pthread_mutex_unlock(&mutex); // mutex == 1
		sleep(rand() % 3);
	}
	//销毁互斥锁
	pthread_mutex_destroy(&mutex);
	return 0;
}
