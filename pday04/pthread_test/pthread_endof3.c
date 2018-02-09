#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void *tfn1(void* arg)
{
	printf("1th thread running\n");
	return (void*)111;
}

void* tfn2(void* arg)
{
	printf("2th thread exiting\n");
	pthread_exit((void*)222);
}

void* tfn3(void* arg)
{
	while(1)
	{
		//printf("3 seconds later,this thread will dead\n");
		//sleep(1);
		//取消点
		pthread_testcancel();
	}
	return (void*)1;
}

int main(void)
{
	pthread_t tid;
	int ret;	
	int *pret;
	ret = pthread_create(&tid,NULL,tfn1,NULL);
	if(ret != 0)
	{
		fprintf(stderr,"pthread_create:%s\n",strerror(ret));
		exit(ret);
	}
	pthread_join(tid,(void**)&pret);
	printf("1th thread exit code with %d\n",(int)pret);
	
	ret = pthread_create(&tid,NULL,tfn2,NULL);
	if(ret != 0)
	{
		fprintf(stderr,"pthread_create:%s\n",strerror(ret));
		exit(ret);
	}
	pthread_join(tid,(void**)&pret);
	printf("2th thread exit code with %d\n",(int)pret);

	ret = pthread_create(&tid,NULL,tfn3,NULL);
	if(ret != 0)
	{
		fprintf(stderr,"pthread_create:%s\n",strerror(ret));
		exit(ret);
	}
	sleep(3);
	//杀死线程
	pthread_cancel(tid);
	//回收线程
	pthread_join(tid,(void**)&pret);
	printf("3th thread exit code with %d\n",(int)pret);
	return 0;
}
