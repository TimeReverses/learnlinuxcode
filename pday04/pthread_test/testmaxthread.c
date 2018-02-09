#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void* tfn(void* arg){
	while(1)
		sleep(1);
}


int main(void)
{
	pthread_t tid;
	int ret;
	int num = 0;
	while(1)
	{
		ret = pthread_create(&tid,NULL,tfn,NULL);
		num++;
		if(ret != 0)
		{
			printf("max size is %d\n",num);
			fprintf(stderr,"pthread_create:%s,%d\n",strerror(ret),ret);
			exit(1);
		}
	}
	return 0;
}
