#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* thread_func(void* arg)
{
	pthread_t tid = pthread_self();
	printf("In thread : thread id = %lu, pid = %d\n",tid,getpid());
//	sleep(59);
	return NULL;
}

int main(void)
{
	pthread_t tid;
	printf("In main thread : thread id = %lu, pid = %d\n",pthread_self(), getpid());
	int ret;
	ret = pthread_create(&tid,NULL,thread_func,NULL);
	if(ret != 0)
	{
		fprintf(stderr,"pthread_create error:%s\n",strerror(ret));
		exit(ret);
	}
	sleep(1);
	printf("In main thread : thread id = %lu, pid = %d\n",pthread_self(), getpid());
	return 0;
}
