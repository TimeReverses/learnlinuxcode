#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int var = 100;//全局变量

void* thread_do(void* msg)
{
	//打印全局变量的值	
	printf("thread   -------------   var before changed = %d\n",var);
	var = 200;
	printf("thread   -------------   var after changed = %d\n",var);
	return NULL;
}

int main(void)
{
	//创建一个新线程
	pthread_t tid;
	int ret = pthread_create(&tid,NULL,thread_do,NULL);
	printf("var before changed = %d\n",var);
	if(ret != 0)
	{
		fprintf(stderr,"pthread_create:%s\n",strerror(ret));
		exit(ret);
	}
	sleep(1);
	//在主线程中打印全局变量的值
	printf("var after changed = %d\n",var);
	return 0;
}
