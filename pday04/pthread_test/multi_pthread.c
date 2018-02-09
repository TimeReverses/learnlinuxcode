#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void* thread_func(void* arg)
{
	//获取传过来的值
	//将void* 强制转换为int类型,在高位上截取
	int msg = (int)arg;
	sleep(msg);
	printf("In %dth thread: thread id : %lu,pid = %u\n",msg+1,pthread_self(),getpid());	
	return NULL;
}

int main(void)
{
	pthread_t tid,i;
	//循环创建多个子线程
	for(i = 0;i < 5;i++)
	{
		//这里传递i使用的是值传递的方式,将4byte的int类型的i,转换为8字节void* 类型
		//会自动在高位补0
		int ret = pthread_create(&tid,NULL,thread_func,(void*)i);
		if(ret != 0)
		{
			fprintf(stderr,"pthread_create error:%s\n",strerror(ret));
			exit(ret);
		}
	}
	sleep(60);
	printf("In main thread : thread id = %lu, pid = %d\n",pthread_self(), getpid());
	return 0;//这里的return是表示整个进程结束,该进程的所有线程也会结束
}
