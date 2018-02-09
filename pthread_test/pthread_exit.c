#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void* pthread_func(void* msg)
{
	printf("In thread pthread id : %lu,pid : %u\n",pthread_self(),getpid());
	//return NULL;
	//pthread_exit(NULL);
	exit(1);
}

int main(void)
{
	//主进程,主线程
	//创建一个新线程
	pthread_t tid;
	int ret = pthread_create(&tid,NULL,pthread_func,NULL);
	if(ret != 0)
	{
		fprintf(stderr,"pthread_create:%s\n",strerror(ret));
		exit(ret);//exit的作用不论在哪个线程或进程中,都是为了退出进程
	}
	sleep(1);
	printf("In main pthread id : %lu,pid : %u\n",pthread_self(),getpid());
	pthread_exit(NULL);//退出当前线程
	//return 0;
	//exit(1);//退出当前进程
}
