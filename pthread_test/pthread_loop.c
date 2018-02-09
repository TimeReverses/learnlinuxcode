#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

int var = 100;

void* ptf(void * arg)
{
	//首先获取参数的值
	int i;
	if(arg != NULL)
		i = (int)arg;
	if(i == 1)
	{
		var = 333;
		return (void*)var;
	}else if(i == 3)
	{
		var = 777;
		pthread_exit((void*)var);
	}else
	{
		pthread_exit((void*)var);
	}
}

int main(void)
{
	pthread_t tid[5];
	int f;
	int* ret[5];//回收线程是的信息
	//创建多个线程
	for(int i = 0;i<5;i++)
	{
		f = pthread_create(&tid[i],NULL,ptf,(void*)i);
		if(f != 0)
		{
			fprintf(stderr,"pthread_create:%s\n",strerror(f));
			exit(f);
		}
	}
	//回收多个线程
	for(int i=0;i<5;i++)
	{
		pthread_join(tid[i],(void**)&ret[i]);
		printf("-------%dth thread ret is %d\n",i,(int)ret[i]);
	}
	printf("In main,byebye\n");
	return 0;
}
