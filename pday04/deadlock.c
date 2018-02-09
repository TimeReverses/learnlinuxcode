#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex1;//锁1
pthread_mutex_t mutex2;//锁2

//共享数据
int var1 = 10;
int var2 = 100;

void* tfn(void* arg)
{
	//上锁2
	pthread_mutex_lock(&mutex2);
	sleep(1);//等待main线程上锁1
	printf("sub %d\n",var2);
	//上锁1
	pthread_mutex_lock(&mutex1);
	printf("sub %d\n",var1);
	pthread_mutex_unlock(&mutex2);
	pthread_mutex_unlock(&mutex1);
	pthread_exit((void*)1);
}

int main(void)
{
	//初始化锁
	pthread_mutex_init(&mutex1,NULL);
	pthread_mutex_init(&mutex2,NULL);
	//创建新线程
	pthread_t tid;
	pthread_create(&tid,NULL,tfn,NULL);
	while(1)
	{
		//上锁1
		pthread_mutex_lock(&mutex1);
		sleep(1);//等待子线程上锁2
		printf("main %d\n",var1);
		//上锁2
		pthread_mutex_lock(&mutex2);
		printf("main %d\n",var2);
		pthread_mutex_unlock(&mutex1);
		pthread_mutex_unlock(&mutex2);
	}

	return 0;
}
