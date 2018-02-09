#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM 5
//创建两个信号量
sem_t product_num,customer_num;
//共享数据
int que[NUM];

void* producter(void* arg)
{
	int i = 0;
	while(1)
	{
		//将空白处减1,如果为0,阻塞等待
		sem_wait(&customer_num);
		que[i] = rand() % 1000 + 1;
		printf("-----producter : %d\n",que[i]);
		sem_post(&product_num);//通知产品加1
		
		i= (i + 1) % NUM;
		sleep(rand() % 3);
	}
	pthread_exit((void*)1);
}

void* customer(void* arg)
{
	int i = 0;
	while(1)
	{
		//将产品量减1,如果为0,阻塞等待
		sem_wait(&product_num);
		printf("----customer : %d\n",que[i]);
		que[i] = 0;//消费产品
		sem_post(&customer_num);//通知生产者,消费量增加1
		
		i = (i + 1) % NUM;
		usleep(rand() % 3);
	}
	pthread_exit((void*)1);
}

int main(void)
{
	srand(time(NULL));
	//初始化信号量
	sem_init(&product_num,0,0);
	sem_init(&customer_num,0,NUM);
	//创建新线程
	pthread_t ptid,ctid;
	pthread_create(&ptid,NULL,producter,NULL);
	pthread_create(&ctid,NULL,customer,NULL);
	//回收子线程
	pthread_join(ptid,NULL);
	pthread_join(ctid,NULL);
	//销毁信号量
	sem_destroy(&product_num);
	sem_destroy(&customer_num);
	return 0;
}
