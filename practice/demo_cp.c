#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

void sys_err(char* msg)
{
	fprintf(stderr,"%s\n",msg);
	exit(1);
}

typedef struct
{
	char* destaddr;
	char* srcaddr;
	int size;
}info;
#define SIZE 4096
void copy(info* inf)
{
	int size = inf->size;
	char temp[SIZE] = {0};
	int time = size / SIZE + 1;
	int lastsize = size - (time -1)*SIZE;
	for(int i=0;i<time;i++)
	{
		memset(temp,0,SIZE);
		char* dp = inf->destaddr + SIZE * i;
		char* sp = inf->srcaddr + SIZE * i;
		if(i == time -1)
			memcpy(dp,sp,lastsize);
		else
			memcpy(dp,sp,SIZE);
		putchar('#');
		fflush(stdout);
		sleep(1);
	}
}

void* thread_func(void* arg)
{
	info* inf = (info*)arg;
	copy(inf);	
	free(inf);
	pthread_exit((void*)1);
}

int main(int args,char* argv[])
{
	if(args != 4)
	{
		sys_err("./a.out srcfile destfile threadnum");
	}
	char srcname[128] = {0};
	char destname[128] = {0};
	int tnum;
	strcpy(srcname,argv[1]);
	strcpy(destname,argv[2]);
	tnum = atoi(argv[3]);
	int fd;
	//打开源文件
	fd = open(srcname,O_RDWR);
	if(fd == -1)
		sys_err("open srcfile error");
	//获取文件的大小
	int size = lseek(fd,0,SEEK_END);
	//返回文件头
	lseek(fd,0,SEEK_SET);
	//为源文件创建mmap映射区
	char* srcp = mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(srcp == MAP_FAILED)
		sys_err("mmap error");
	//关闭文件
	close(fd);
	//打开目标文件
	int dfd = open(destname,O_CREAT|O_RDWR|O_TRUNC,0644);
	if(dfd == -1)
		sys_err("open destfile error");
	ftruncate(dfd,size);//为目标文件拓展空间
	char* desp = mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,dfd,0);
	if(desp == MAP_FAILED)
		sys_err("dest mmap error");
	//分配每一个线程需要拷贝的大小
	int avgsize = size / tnum;
	int lastsize = size - avgsize * tnum + avgsize;
	pthread_t tid;
	int ret;
	//创建线程
	for(int i=0;i<tnum;i++)
	{
		char* srctemp = srcp + i*avgsize;
		char* destemp = desp + i*avgsize;
		info* inf = (info*)malloc(sizeof(info));
		inf->destaddr = destemp;
		inf->srcaddr = srctemp;
		inf->size = avgsize;
		if(i == tnum -1)
		{
			inf->size = lastsize;
			ret = pthread_create(&tid,NULL,thread_func,(void*)inf);
			//设置线程分离
			pthread_detach(tid);
		}
		else
		{
			ret = pthread_create(&tid,NULL,thread_func,(void*)inf);
			//设置线程分离
			pthread_detach(tid);
		}
		if(ret != 0)
		{
			char temp[128];
			sprintf(temp,"pthread_create:%s",strerror(ret));
			sys_err(temp);
		}
	}
	pthread_exit((void*)1);
	//主线程负责打印进度条
	//sleep(10);
	//return 0;
}
