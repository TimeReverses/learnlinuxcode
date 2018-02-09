#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>

//信号量
sem_t input;

void* inputstream(void* arg)
{
	char buf[1024] = {0};
	while(1)
	{
		memset(buf,0,sizeof(buf));
		//阻塞等待用户输入
		fgets(buf,sizeof(buf),stdin);
		sem_post(&input);//使信号量加1
	}
	pthread_exit(NULL);
}
/*
   struct timespec {
   time_t tv_sec;      
   long   tv_nsec;      
   };
 */

void* outputstream(void* arg)
{
	while(1)
	{
		struct timespec ts;
		int cur = time(NULL);
		ts.tv_sec = cur + 5; 
		sem_timedwait(&input,&ts);//定时5秒
		fputs("hello world\n",stdout);
	}
	pthread_exit(NULL);
}

int main(void)
{
	pthread_t ptid,ctid;
	//初始化信号量
	sem_init(&input,0,0);
	//创建线程
	pthread_create(&ptid,NULL,inputstream,NULL);
	pthread_create(&ctid,NULL,outputstream,NULL);
	//回收线程
	pthread_join(ptid,NULL);
	pthread_join(ctid,NULL);
	//销毁信号量
	sem_destroy(&input);
	return 0;
}
