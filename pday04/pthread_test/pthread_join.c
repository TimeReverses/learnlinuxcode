#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

typedef struct
{
	char name[64];
	int age;
	char gender;
}exit_t;

void* thread_func(void* arg)
{
	exit_t* retval = (exit_t*)malloc(sizeof(exit_t));
	strcpy(retval->name,"桃乃木香奈");
	retval->age = 22;
	retval->gender = 'f';
	//线程退出
	pthread_exit((void*)retval);
}

int main(void)
{
	pthread_t tid;
	int ret;
	exit_t* retval;
	//创建一个新线程
	ret = pthread_create(&tid,NULL,thread_func,NULL);
	if(ret != 0)
	{
		fprintf(stderr,"pthread_create:%s\n",strerror(ret));
		exit(ret);
	}
	//回收子线程,pthread_join会阻塞等待子线程
	pthread_join(tid,(void**)&retval);
	if(retval != NULL)
	{
		printf("name = %s,age = %d,gender = %c\n",retval->name,retval->age,retval->gender);
		free(retval);
	}
	return 0;
}
