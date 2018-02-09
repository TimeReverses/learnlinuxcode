#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void* tfn(void* arg)
{
	printf("thread id is %lu and pid is %u\n",pthread_self(),getpid());
	printf("thread is running and it will be dead\n");
	printf("---------------------\n");
	//退出线程
	pthread_exit((void*)1);
}

int main(void)
{
	pthread_t tid;
	int ret;
	pthread_attr_t attr;
	//初始化属性变量
	pthread_attr_init(&attr);
	//设置分离状态给属性变量
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	//创建一个新线程
	ret = pthread_create(&tid,&attr,tfn,NULL);
	if(ret != 0)
	{
		fprintf(stderr,"pthread_create:%s\n",strerror(ret));
		exit(1);
	}
	//销毁属性变量所占用的空间
	pthread_attr_destroy(&attr);
	//回收线程
	sleep(1);
	ret = pthread_join(tid,NULL);
	if(ret != 0)
	{
		fprintf(stderr,"pthread_join:%s\n",strerror(ret));
		exit(1);
	}
	return 0;
}

