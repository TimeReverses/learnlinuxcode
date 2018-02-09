#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

void sys_err(char *str)
{
	perror(str);
	exit(1);
}

typedef struct STU
{
	int age;
	char name[20];
	char sex;
}STU;
int main(int args,char* argv[])
{
	if(args < 2)
		sys_err("./a.out filename");
	int fd = open(argv[1],O_RDWR);
	if(fd == -1)
		sys_err("file open");
	STU student;
	STU* mm;
	//创建一个映射区
	mm = mmap(NULL,sizeof(student),PROT_READ,MAP_SHARED,fd,0);//只读
	if(mm == MAP_FAILED)
		sys_err("mmap error");
	close(fd);
	for(int i=0;i<10;i++){
	STU *temp = mm;
	for(int i=0;i<2;i++,temp++)
	{
		sleep(1);//每隔1秒读取数据一次
		printf("age:%d,name=%s,sex=%c\n",temp->age,temp->name,temp->sex);
	}
	}
	//printf("age:%d,name=%s,sex=%c\n",temp->age,temp->name,temp->sex);
	//关闭映射区
	int ret = munmap(mm,sizeof(student));
	if(ret == -1)	
		sys_err("munmap");
	return 0;
}
