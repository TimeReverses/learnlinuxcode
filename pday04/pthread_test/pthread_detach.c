#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void* pthread_func(void* arg)
{
	int n = 3;
	while(n--){
		sleep(1);
		printf("count = %d\n",n);
	}
	pthread_exit((void*)1);
}

int main(void)
{
	pthread_t tid;
	int ret;
	int* pret;
	//创建一个新线程
	ret = pthread_create(&tid,NULL,pthread_func,NULL);
	if(ret != 0)
	{
		fprintf(stderr,"pthread_create:%s\n",strerror(ret));
		exit(ret);
	}
	//分离线程
	ret = pthread_detach(tid);
	if(ret != 0)
	{
		fprintf(stderr,"pthread_detach:%s\n",strerror(ret));
		exit(ret);
	}
	//主线程回收线程
	while(1)
	{
		ret = pthread_join(tid,(void**)&pret);
		if(ret != 0)
		{
			fprintf(stderr,"pthread_join error:%s\n",strerror(ret));
		}
		else
		{
			fprintf(stderr,"exit code : %d\n",(int)pret);
		}
		sleep(1);
	}
	return 0;
}
