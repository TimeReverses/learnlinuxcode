#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

//声明并初始化 互斥变量 和 条件变量
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct msg
{
	//指针域
	struct msg* next;
	//数据域
	int num;
};

//共享资源
struct msg* head;
struct msg* mg;

void* product(void* arg)
{
	while(1)
	{
		//创建一个产品
		mg = (struct msg*)malloc(sizeof(struct msg));
		mg->num = rand() % 1000 + 1;
		//上锁
		pthread_mutex_lock(&mutex);
		//添加产品到公共资源中
		mg->next = head;
		head = mg;
		//释放锁
		pthread_mutex_unlock(&mutex);
		//打印生产信息
		printf("product ------- %d\n",mg->num);
		//发出广播
		pthread_cond_signal(&cond);
		sleep(rand() % 5);
	}
	pthread_exit((void*)1);
}

void* customer(void* arg)
{
	while(1)
	{
		//上锁
		pthread_mutex_lock(&mutex);
		while(head == NULL)
		{
			pthread_cond_wait(&cond,&mutex);
		}
		//消费产品
		mg = head;
		head = mg->next;
		//释放锁
		pthread_mutex_unlock(&mutex);
		//打印消费的信息
		printf("customer -------- %d\n",mg->num);
		free(mg);
		sleep(rand() % 5);
	}
	pthread_exit((void*)1);
}

int main(void)
{
	srand(time(NULL));
	pthread_t pid,cid;
	//创建顾客线程和生产者线程
	pthread_create(&pid,NULL,product,NULL);
	pthread_create(&cid,NULL,customer,NULL);
	//主线程回收子线程
	pthread_join(pid,NULL);
	pthread_join(cid,NULL);
	return 0;
}
